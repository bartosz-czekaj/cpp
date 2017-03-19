#include <stdio.h>
#include <stdlib.h>

int solution(int N) 
{
    int iNumber = N;
    int iRest = 0;

    int iCurLen = 0;
    int iMaxLen = 0;
    int iWasOne = 0;
    
    do 
    {
        iRest = iNumber % 2;
        if (iRest == 1)
        {
          if(iMaxLen < iCurLen)
              iMaxLen = iCurLen;
          iCurLen = 0;
          iWasOne = 1;
        }
        else 
        {
          if(iWasOne == 1)  
             iCurLen++;  
        }
    
        iNumber = iNumber / 2;
    }while (iNumber > 0);

    return iMaxLen;
}