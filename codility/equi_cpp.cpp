#include <vector>
#include <iostream>
#include <map> 
#include <algorithm>
using namespace std;

int solution(vector<int> &A) 
{
  int iRet = 0;
  std::map<int, int> mapCount;
  
  for(std::vector<int>::iterator it = A.begin(); it != A.end(); ++it)
  {
      //std::cout<<*it<<endl;
      mapCount[*it]++;
  }
  
  int iLeader = 0;
  int iLeaderCount = 0;
  bool bLeader = false;
  for(std::map<int,int>::iterator it = mapCount.begin(); it != mapCount.end(); ++it)
  {
      if(it->second > A.size() / 2)
      {
          iLeaderCount = it->second;
          iLeader = it->first;
          bLeader = true;
          break;
      }
  }
  int iIdx = 0;
  int iIle1 = 0, iIle2 = 0;
  int iCounter = 0;
  int iMid = A.size() / 2;
  for(iIdx = 0; iIdx < iMid && bLeader; ++iIdx, ++iCounter)
  {
      if(A[iIdx] == iLeader)
          ++iIle1;
      if(A[A.size() - 1- iIdx] == iLeader)
          ++iIle2;
      
      if((iCounter + 1) / 2 < iIle1 && ((iLeaderCount - iIle1) > (A.size() - (iCounter + 1)) - (iLeaderCount - iIle1)))
      {
          iRet++;
      }
      if((iCounter + 1 < A.size() / 2) && ((iCounter + 1) / 2 < iIle2) && (A.size() - 1- iIdx != iIdx))
      {
          if((A.size() - (iCounter + 1)) - (iLeaderCount - iIle2) < (iLeaderCount - iIle2))
                iRet++;
      }
  }
  return iRet;

}