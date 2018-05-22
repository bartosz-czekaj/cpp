#include <iostream>

bool isLittleEndian()
{
    short int number = 0x1;
    char *numPtr = (char*)&number;
    return (numPtr[0] == 1);
}

int main()
{
    std::cout<<isLittleEndian<<'\n';
}