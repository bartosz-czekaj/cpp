#include "stdafx.h"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <iphlpapi.h>
#include "Mstcpip.h"
#include <Tlhelp32.h> 
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <sstream> 
#include <iomanip> 
#include <time.h>

#pragma comment(lib, "ws2_32.lib") //For winsock
#pragma comment(lib, "iphlpapi.lib")

auto convertCharArrayToLPCWSTR = [](char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
};


#define SIO_RCVALL  _WSAIOW(IOC_VENDOR,1)

enum class workingmode
{
	OTHER_MODE = 0,
	TCP_MODE = 1,
	UPD_MODE = 2
};

void ProcessPacket(char*, int); //This will decide how to digest
void PrintIpHeader(char*, workingmode mode, unsigned short sourcePort, unsigned short destPort);
void PrintIcmpPacket(char*, int);
void PrintUdpPacket(char*, int);
void PrintTcpPacket(char*, int);
void ConvertToHex(char*, unsigned int);
void StartSniffing(SOCKET sniffer);
void PrintData(char*, int);
int GetProcessNameFromTCPTable(std::wstring &pName, unsigned short portID);
int GetProcessNameFromUDPTable(std::wstring &pName, unsigned short portID);

typedef struct ip_hdr
{
	unsigned char ip_header_len : 4; // 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)
	unsigned char ip_version : 4; // 4-bit IPv4 version
	unsigned char ip_tos; // IP type of service
	unsigned short ip_total_length; // Total length
	unsigned short ip_id; // Unique identifier

	unsigned char ip_frag_offset : 5; // Fragment offset field

	unsigned char ip_more_fragment : 1;
	unsigned char ip_dont_fragment : 1;
	unsigned char ip_reserved_zero : 1;

	unsigned char ip_frag_offset1; //fragment offset

	unsigned char ip_ttl; // Time to live
	unsigned char ip_protocol; // Protocol(TCP,UDP etc)
	unsigned short ip_checksum; // IP checksum
	unsigned int ip_srcaddr; // Source address
	unsigned int ip_destaddr; // Source address
} IPV4_HDR;

typedef struct udp_hdr
{
	unsigned short source_port; // Source port no.
	unsigned short dest_port; // Dest. port no.
	unsigned short udp_length; // Udp packet length
	unsigned short udp_checksum; // Udp checksum (optional)
} UDP_HDR;

// TCP header
typedef struct tcp_header
{
	unsigned short source_port; // source port
	unsigned short dest_port; // destination port
	unsigned int sequence; // sequence number - 32 bits
	unsigned int acknowledge; // acknowledgement number - 32 bits

	unsigned char ns : 1; //Nonce Sum Flag Added in RFC 3540.
	unsigned char reserved_part1 : 3; //according to rfc
	unsigned char data_offset : 4; /*The number of 32-bit words in the TCP header.
								   This indicates where the data begins.
								   The length of the TCP header is always a multiple
								   of 32 bits.*/

	unsigned char fin : 1; //Finish Flag
	unsigned char syn : 1; //Synchronise Flag
	unsigned char rst : 1; //Reset Flag
	unsigned char psh : 1; //Push Flag
	unsigned char ack : 1; //Acknowledgement Flag
	unsigned char urg : 1; //Urgent Flag

	unsigned char ecn : 1; //ECN-Echo Flag
	unsigned char cwr : 1; //Congestion Window Reduced Flag

						   ////////////////////////////////

	unsigned short window; // window
	unsigned short checksum; // checksum
	unsigned short urgent_pointer; // urgent pointer
} TCP_HDR;

typedef struct icmp_hdr
{
	BYTE type; // ICMP Error type
	BYTE code; // Type sub code
	USHORT checksum;
	USHORT id;
	USHORT seq;
} ICMP_HDR;

FILE *logfile;
int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0, i, j;
struct sockaddr_in source, dest;
char hex[2];

//Its free!
IPV4_HDR *iphdr;
TCP_HDR *tcpheader;
UDP_HDR *udpheader;
ICMP_HDR *icmpheader;
std::string myIP;

int main(void)
{
	WSADATA wsaData;
	SOCKET sock;
	struct sockaddr_in sin;
	int flag = 1;
	DWORD ret;

	fopen_s(&logfile, "log.txt", "w");
	if (logfile == nullptr)
	{
		printf("Unable to create file.");
		return -1;
	}


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		//printf("error start: %d\n", WSAGetLastError());
		return -1;
	}

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (sock == INVALID_SOCKET)
	{
		//printf("error socket: %d\n", WSAGetLastError());
		return -1;
	}

	/////////////////////
	char hostname[255];

	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		std::cout << "Failed to get hostname." << '\n';
		std::cout << WSAGetLastError() << '\n';
		return -1;
	}

	struct addrinfo *res = nullptr;
	DWORD dwRetval = getaddrinfo(hostname, nullptr, 0, &res);


	if (dwRetval != 0)
	{
		std::cout << "getaddrinfo failed with error: " << dwRetval << '\n';
		std::cout << WSAGetLastError() << '\n';
		WSACleanup();
		return -1;
	}

	if (res == nullptr)
	{
		std::cout << "Error: " << WSAGetLastError() << '\n';
		return -1;
	}
	struct addrinfo *ptr = nullptr;
	//struct in_addr addr;
	std::vector<addrinfo *> addresses;
	for (ptr = res; ptr != nullptr; ptr = ptr->ai_next)
	{
		if (ptr->ai_family == AF_INET)
		{
			char sstr[INET_ADDRSTRLEN];
			

			struct sockaddr_in  *sockaddr_ipv4 = nullptr;
			addresses.emplace_back(ptr);
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), sstr, INET_ADDRSTRLEN);

			myIP = sstr;
			break;
		}
	}

	/////////////////////////

	memset(&sin, 0x00, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = 0;
	inet_pton(AF_INET, myIP.c_str(), &sin.sin_addr);

	if (bind(sock, (struct sockaddr*)&sin, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		//printf("error bind: %d\n", WSAGetLastError());
		return -1;
	}

	if (WSAIoctl(sock, SIO_RCVALL, &flag, sizeof(flag), NULL, 0, &ret, NULL, NULL) == SOCKET_ERROR)
	{
		//printf("error ioctl: %d\n", WSAGetLastError());
		return -1;
	}

	// ok !
	StartSniffing(sock);
	WSACleanup();

	return 0;
}


void StartSniffing(SOCKET sniffer)
{
	char *Buffer = (char *)malloc(65536); //Its Big!
	int mangobyte;

	if (Buffer == NULL)
	{
		printf("malloc() failed.\n");
		return;
	}

	do
	{
		mangobyte = recvfrom(sniffer, Buffer, 65536, 0, 0, 0); //Eat as much as u can

		if (mangobyte > 0)
		{
			ProcessPacket(Buffer, mangobyte);
		}
		else
		{
			printf("recvfrom() failed.\n");
		}
	} while (mangobyte > 0);

	free(Buffer);
}

void ProcessPacket(char* Buffer, int Size)
{
	iphdr = (IPV4_HDR *)Buffer;
	++total;

	switch (iphdr->ip_protocol) //Check the Protocol and do accordingly...
	{
	case 1: //ICMP Protocol
		++icmp;
		PrintIcmpPacket(Buffer, Size);
		break;

	case 2: //IGMP Protocol
		++igmp;
		break;

	case 6: //TCP Protocol
		++tcp;
		PrintTcpPacket(Buffer, Size);
		break;

	case 17: //UDP Protocol
		++udp;
		PrintUdpPacket(Buffer, Size);
		break;

	default: //Some Other Protocol like ARP etc.
		++others;
		break;
	}
	printf("TCP : %d UDP : %d ICMP : %d IGMP : %d Others : %d Total : %d\r", tcp, udp, icmp, igmp, others, total);
}

std::string DataAsString(const std::chrono::system_clock::time_point& tp)
{
	char str[32];
	time_t result = time(NULL);
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	ctime_s(str, sizeof str, &result);
	std::string ts = str;
	ts.resize(ts.size() - 1);
	return ts;
}

void PrintIpHeader(char* Buffer, workingmode mode, unsigned short sourcePort, unsigned short destPort)
{
	unsigned short iphdrlen;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iphdr->ip_srcaddr;

	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iphdr->ip_destaddr;

	char sstr[INET_ADDRSTRLEN];
	char dstr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(source.sin_addr), sstr, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(dest.sin_addr), dstr, INET_ADDRSTRLEN);

	auto start = std::chrono::system_clock::now();
	std::time_t packetTime = std::chrono::system_clock::to_time_t(start);

	fprintf(logfile, "\n");
	fprintf(logfile, "Time\n");
	fprintf(logfile, " |-Packet now [ms]: %d\n", std::chrono::time_point_cast<std::chrono::milliseconds>(start));
	fprintf(logfile, " |-Packet local time: [%s]\n", DataAsString(start).c_str());
	fprintf(logfile, "\n");

	fprintf(logfile, "\n");
	fprintf(logfile, "IP Header\n");
	fprintf(logfile, " |-IP Version : %d\n", (unsigned int)iphdr->ip_version);
	fprintf(logfile, " |-IP Header Length : %d DWORDS or %d Bytes\n", (unsigned int)iphdr->ip_header_len, ((unsigned int)(iphdr->ip_header_len)) * 4);
	fprintf(logfile, " |-Type Of Service : %d\n", (unsigned int)iphdr->ip_tos);
	fprintf(logfile, " |-IP Total Length : %d Bytes(Size of Packet)\n", ntohs(iphdr->ip_total_length));
	fprintf(logfile, " |-Identification : %d\n", ntohs(iphdr->ip_id));
	fprintf(logfile, " |-Reserved ZERO Field : %d\n", (unsigned int)iphdr->ip_reserved_zero);
	fprintf(logfile, " |-Dont Fragment Field : %d\n", (unsigned int)iphdr->ip_dont_fragment);
	fprintf(logfile, " |-More Fragment Field : %d\n", (unsigned int)iphdr->ip_more_fragment);
	fprintf(logfile, " |-TTL : %d\n", (unsigned int)iphdr->ip_ttl);
	fprintf(logfile, " |-Protocol : %d\n", (unsigned int)iphdr->ip_protocol);
	fprintf(logfile, " |-Checksum : %d\n", ntohs(iphdr->ip_checksum));
	fprintf(logfile, " |-Source IP : %s\n", sstr);
	fprintf(logfile, " |-Destination IP : %s\n", dstr);

	
	if (mode != workingmode::OTHER_MODE)
	{
		unsigned short port = 0;

		if (strcmp(sstr, myIP.c_str()) == 0)
			port = sourcePort;
		else if (strcmp(dstr, myIP.c_str()) == 0)
			port = destPort;

		std::wstring pName;

		int retValue = 0;

		if(mode == workingmode::TCP_MODE)
			retValue = GetProcessNameFromTCPTable(pName, port);
		else if (mode == workingmode::UPD_MODE)
			retValue = GetProcessNameFromUDPTable(pName, port);

		if (retValue == 0)
		{
			fprintf(logfile, "\n");
			fprintf(logfile, "Processes\n");
			fprintf(logfile, " |-Process id: %d\n", port);
			fprintf(logfile, " |-Process name: %ls\n", pName.c_str());
			fprintf(logfile, "\n");
		}
	}

}

int GetProcName(DWORD aPid, std::wstring &procName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot) 
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &pe32)) 
		{
			do 
			{
				if (pe32.th32ProcessID == aPid)
				{
					procName = pe32.szExeFile;
					//printf("pid %d %s\n", pe32.th32ProcessID, pe32.szExeFile);
					break;
				}
			} while (Process32Next(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);

		return 0;
	}
}

int GetProcName_old(DWORD aPid, std::wstring &procName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);
	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	for (BOOL bok = Process32First(processesSnapshot, &processInfo); bok; bok = Process32Next(processesSnapshot, &processInfo))
	{
		if (aPid == processInfo.th32ProcessID)
		{
			CloseHandle(processesSnapshot);
			procName = processInfo.szExeFile;
			return 0;
		}

	}
	CloseHandle(processesSnapshot);
	return -1;
}

int GetProcessNameFromUDPTable(std::wstring &pName, unsigned short portID)
{
	if (portID == 0)
		return -1;

	DWORD(WINAPI *pGetExtendedUdpTable)(
		PVOID pUdpTable,
		PDWORD pdwSize,
		BOOL bOrder,
		ULONG ulAf,
		UDP_TABLE_CLASS TableClass,
		ULONG Reserved
		);

	MIB_UDPTABLE_OWNER_PID *pUDPInfo;
	MIB_UDPROW_OWNER_PID *owner;
	DWORD size;
	DWORD dwResult;

	HMODULE hLib = LoadLibrary(convertCharArrayToLPCWSTR("iphlpapi.dll"));


	pGetExtendedUdpTable = (DWORD(WINAPI *)(PVOID, PDWORD, BOOL, ULONG, UDP_TABLE_CLASS, ULONG))
		GetProcAddress(hLib, "GetExtendedUdpTable");


	if (!pGetExtendedUdpTable)
	{
		return -1;
	}


	dwResult = pGetExtendedUdpTable(NULL, &size, false, AF_INET, UDP_TABLE_BASIC, 0);
	pUDPInfo =  (MIB_UDPTABLE_OWNER_PID*)malloc(size);
	dwResult = pGetExtendedUdpTable(pUDPInfo, &size, false, AF_INET, UDP_TABLE_BASIC, 0);

	if (dwResult != NO_ERROR)
	{
		return -1;
	}

	for (DWORD dwLoop = 0; dwLoop < pUDPInfo->dwNumEntries; dwLoop++)
	{
		owner = &pUDPInfo->table[dwLoop];
		if (!owner)
			continue;

		if (owner->dwLocalPort == portID)
		{
 			GetProcName(owner->dwOwningPid, pName);
			break;
		}
	}

	return 0;
}

int GetProcessNameFromTCPTable(std::wstring &pName, unsigned short portID)
{
	if (portID == 0)
		return -1;

	DWORD(WINAPI *pGetExtendedTcpTable)(
		PVOID pTcpTable,
		PDWORD pdwSize,
		BOOL bOrder,
		ULONG ulAf,
		TCP_TABLE_CLASS TableClass,
		ULONG Reserved
		);
	MIB_TCPTABLE_OWNER_PID *pTCPInfo;
	MIB_TCPROW_OWNER_PID *owner;
	DWORD size;
	DWORD dwResult;


	HMODULE hLib = LoadLibrary(convertCharArrayToLPCWSTR("iphlpapi.dll"));

	pGetExtendedTcpTable = (DWORD(WINAPI *)(PVOID, PDWORD, BOOL, ULONG, TCP_TABLE_CLASS, ULONG))
		GetProcAddress(hLib, "GetExtendedTcpTable");

	dwResult = pGetExtendedTcpTable(NULL, &size, false, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	pTCPInfo = (MIB_TCPTABLE_OWNER_PID*)malloc(size);
	dwResult = pGetExtendedTcpTable(pTCPInfo, &size, false, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	for (DWORD dwLoop = 0; dwLoop < pTCPInfo->dwNumEntries; dwLoop++)
	{
		owner = &pTCPInfo->table[dwLoop];
		if (!owner)
			continue;
		if (owner->dwLocalPort == portID || owner->dwRemotePort == portID)
		{
			GetProcName(owner->dwOwningPid, pName);
			break;
		}
	}
	return 0;
}

void PrintTcpPacket(char* Buffer, int Size)
{
	/*PMIB_TCPTABLE2 pTcpTable;
	ULONG ulSize = 0;
	DWORD dwRetVal = 0;*/

	///////*pTcpTable = (MIB_TCPTABLE2 *)malloc(sizeof(MIB_TCPTABLE2));
	//////if (pTcpTable == nullptr)
	//////{
	//////	printf("Error allocating memory\n");
	//////	return;
	//////}

	//////ulSize = sizeof(MIB_TCPTABLE);*/


	//////////// Make an initial call to GetTcpTable2 to
	//////////// get the necessary size into the ulSize variable
	//////////if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) == ERROR_INSUFFICIENT_BUFFER)
	//////////{
	//////////	free(pTcpTable);
	//////////	pTcpTable = (MIB_TCPTABLE2 *)malloc(ulSize);
	//////////	if (pTcpTable == nullptr)
	//////////	{
	//////////		printf("Error allocating memory\n");
	//////////		return;
	//////////	}
	//////////}

	//DWORD pid = 1;//0;

	//////////// Make a second call to GetTcpTable2 to get
	//////////// the actual data we require
	//////////if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) == NO_ERROR)
	//////////{
	//////////	for (i = 0; i < (int)pTcpTable->dwNumEntries; i++)
	//////////	{
	//////////		char lstr[INET_ADDRSTRLEN];
	//////////		char rstr[INET_ADDRSTRLEN];
	//////////		inet_ntop(AF_INET, &(pTcpTable->table[i].dwLocalAddr), lstr, INET_ADDRSTRLEN);
	//////////		inet_ntop(AF_INET, &(pTcpTable->table[i].dwRemoteAddr), rstr, INET_ADDRSTRLEN);

	//////////		if (strcmp(lstr, myIP.c_str()) == 0 || strcmp(rstr, myIP.c_str()) == 0)
	//////////		{
	//////////			pid = pTcpTable->table[i].dwOwningPid;
	//////////		}
	//////////	}
	//////////}


	unsigned short iphdrlen;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	tcpheader = (TCP_HDR*)(Buffer + iphdrlen);
	fprintf(logfile, "\n\n***********************TCP Packet*************************\n");

	PrintIpHeader(Buffer, workingmode::TCP_MODE, tcpheader->source_port, tcpheader->dest_port);

	

	fprintf(logfile, "\n");
	fprintf(logfile, "TCP Header\n");
	fprintf(logfile, " |-Source Port : %u\n", ntohs(tcpheader->source_port));
	fprintf(logfile, " |-Destination Port : %u\n", ntohs(tcpheader->dest_port));
	fprintf(logfile, " |-Sequence Number : %u\n", ntohl(tcpheader->sequence));
	fprintf(logfile, " |-Acknowledge Number : %u\n", ntohl(tcpheader->acknowledge));
	fprintf(logfile, " |-Header Length : %d DWORDS or %d BYTES\n"
		, (unsigned int)tcpheader->data_offset, (unsigned int)tcpheader->data_offset * 4);
	fprintf(logfile, " |-CWR Flag : %d\n", (unsigned int)tcpheader->cwr);
	fprintf(logfile, " |-ECN Flag : %d\n", (unsigned int)tcpheader->ecn);
	fprintf(logfile, " |-Urgent Flag : %d\n", (unsigned int)tcpheader->urg);
	fprintf(logfile, " |-Acknowledgement Flag : %d\n", (unsigned int)tcpheader->ack);
	fprintf(logfile, " |-Push Flag : %d\n", (unsigned int)tcpheader->psh);
	fprintf(logfile, " |-Reset Flag : %d\n", (unsigned int)tcpheader->rst);
	fprintf(logfile, " |-Synchronise Flag : %d\n", (unsigned int)tcpheader->syn);
	fprintf(logfile, " |-Finish Flag : %d\n", (unsigned int)tcpheader->fin);
	fprintf(logfile, " |-Window : %d\n", ntohs(tcpheader->window));
	fprintf(logfile, " |-Checksum : %d\n", ntohs(tcpheader->checksum));
	fprintf(logfile, " |-Urgent Pointer : %d\n", tcpheader->urgent_pointer);
	fprintf(logfile, "\n");
	fprintf(logfile, " DATA Dump ");
	fprintf(logfile, "\n");

	fprintf(logfile, "IP Header\n");
	PrintData(Buffer, iphdrlen);

	fprintf(logfile, "TCP Header\n");
	PrintData(Buffer + iphdrlen, tcpheader->data_offset * 4);

	fprintf(logfile, "Data Payload\n");
	PrintData(Buffer + iphdrlen + tcpheader->data_offset * 4, (Size - tcpheader->data_offset * 4 - iphdr->ip_header_len * 4));

	fprintf(logfile, "\n###########################################################");
}

void PrintUdpPacket(char *Buffer, int Size)
{
	unsigned short iphdrlen;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	udpheader = (UDP_HDR *)(Buffer + iphdrlen);

	fprintf(logfile, "\n\n***********************UDP Packet*************************\n");

	PrintIpHeader(Buffer, workingmode::UPD_MODE, udpheader->source_port, udpheader->dest_port);

	fprintf(logfile, "\nUDP Header\n");
	fprintf(logfile, " |-Source Port : %d\n", ntohs(udpheader->source_port));
	fprintf(logfile, " |-Destination Port : %d\n", ntohs(udpheader->dest_port));
	fprintf(logfile, " |-UDP Length : %d\n", ntohs(udpheader->udp_length));
	fprintf(logfile, " |-UDP Checksum : %d\n", ntohs(udpheader->udp_checksum));

	fprintf(logfile, "\n");
	fprintf(logfile, "IP Header\n");

	PrintData(Buffer, iphdrlen);

	fprintf(logfile, "UDP Header\n");

	PrintData(Buffer + iphdrlen, sizeof(UDP_HDR));

	fprintf(logfile, "Data Payload\n");

	PrintData(Buffer + iphdrlen + sizeof(UDP_HDR), (Size - sizeof(UDP_HDR) - iphdr->ip_header_len * 4));

	fprintf(logfile, "\n###########################################################");
}

void PrintIcmpPacket(char* Buffer, int Size)
{
	unsigned short iphdrlen;

	iphdr = (IPV4_HDR *)Buffer;
	iphdrlen = iphdr->ip_header_len * 4;

	icmpheader = (ICMP_HDR*)(Buffer + iphdrlen);

	fprintf(logfile, "\n\n***********************ICMP Packet*************************\n");
	PrintIpHeader(Buffer, workingmode::OTHER_MODE, 0, 0);

	fprintf(logfile, "\n");

	fprintf(logfile, "ICMP Header\n");
	fprintf(logfile, " |-Type : %d", (unsigned int)(icmpheader->type));

	if ((unsigned int)(icmpheader->type) == 11)
	{
		fprintf(logfile, " (TTL Expired)\n");
	}
	else if ((unsigned int)(icmpheader->type) == 0)
	{
		fprintf(logfile, " (ICMP Echo Reply)\n");
	}

	fprintf(logfile, " |-Code : %d\n", (unsigned int)(icmpheader->code));
	fprintf(logfile, " |-Checksum : %d\n", ntohs(icmpheader->checksum));
	fprintf(logfile, " |-ID : %d\n", ntohs(icmpheader->id));
	fprintf(logfile, " |-Sequence : %d\n", ntohs(icmpheader->seq));
	fprintf(logfile, "\n");

	fprintf(logfile, "IP Header\n");
	PrintData(Buffer, iphdrlen);

	fprintf(logfile, "UDP Header\n");
	PrintData(Buffer + iphdrlen, sizeof(ICMP_HDR));

	fprintf(logfile, "Data Payload\n");
	PrintData(Buffer + iphdrlen + sizeof(ICMP_HDR), (Size - sizeof(ICMP_HDR) - iphdr->ip_header_len * 4));

	fprintf(logfile, "\n###########################################################");
}

/*
Print the hex values of the data
*/
void PrintData(char* data, int Size)
{
	char a, line[17], c;
	int j;

	//loop over each character and print
	for (i = 0; i < Size; i++)
	{
		c = data[i];

		//Print the hex value for every character , with a space. Important to make unsigned
		fprintf(logfile, " %.2x", (unsigned char)c);

		//Add the character to data line. Important to make unsigned
		a = (c >= 32 && c <= 128) ? (unsigned char)c : '.';

		line[i % 16] = a;

		//if last character of a line , then print the line - 16 characters in 1 line
		if ((i != 0 && (i + 1) % 16 == 0) || i == Size - 1)
		{
			line[i % 16 + 1] = '\0';

			//print a big gap of 10 characters between hex and characters
			fprintf(logfile, "          ");

			//Print additional spaces for last lines which might be less than 16 characters in length
			for (j = strlen(line); j < 16; j++)
			{
				fprintf(logfile, "   ");
			}

			fprintf(logfile, "%s \n", line);
		}
	}

	fprintf(logfile, "\n");
}
