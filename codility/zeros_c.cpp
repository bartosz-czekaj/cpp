#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>
#include<iostream>

int iGetZeros(int A_iNumber, int A_iCurrPow, int *A_pData)
{
    int iIdx = 0;
    int iRet = 0;
    int iBase = 10;
    
    
    for(iIdx = 0; iIdx < A_iCurrPow - 1; ++iIdx)
    {
        A_pData[iIdx] = A_iNumber % (int)pow(iBase, (iIdx + 1));
        if(A_pData[iIdx] == 0)
            ++iRet;
    }
    
    
    int iNb = A_pData[A_iCurrPow - 2];
    
    for(iIdx = A_iCurrPow - 3; iIdx >= 0 && iNb > 0; --iIdx)
    {
        if(A_pData[iIdx] == iNb)
            ++iRet;
    }
    
    return iRet;
}

int solution (char *S)
{
    
   int iNumber = atoi(S);
   int iLen = strlen(S);
   int iRet = 0;
   int iIdx = 0;
   int iMaxPow = pow(10, iLen - 1);
   int iCurrPow = 1;
   
   int *pData = 0;
   pData = (int*)calloc(iLen, sizeof(int));
   
   for(iIdx = 1; iIdx<=iNumber; ++iIdx)
   {
       if(pow(10, iCurrPow + 1) == iIdx)
           ++iCurrPow;
       iRet += iGetZeros(iIdx, iCurrPow + 1, pData); 
       
   }
   
   free(pData);
    
   return (iRet + 1) % 1410000017;
}

int main()
{
    char *s = "7777";
  
    std::cout<<"RET: "<<solution(s);
    return 0;
}