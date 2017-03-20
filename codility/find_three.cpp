#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>
#include<iostream>

int solution(char *S) 
{
    int iRet = 0;
    int iLen = strlen(S);
    
    if(iLen < 3)
        return 0;
    
    int iMaxSize = iLen / 3;
    
    char *prefix = (char*) malloc(iMaxSize+1);
    char *sufix = (char*) malloc(iMaxSize+1);
    
    strncpy(prefix, S, iMaxSize);
    strncpy(sufix, S + iMaxSize +(iLen - iMaxSize*2), iMaxSize);
    
   // std::cout<<strlen(prefix)<<std::endl;
    //std::cout<<sufix<<std::endl;
    
    prefix[iMaxSize] ='\0';
    sufix[iMaxSize] ='\0';
    
    int iStart = iMaxSize;
    int iDelta = 0;
    
    for(iStart = iMaxSize; iStart > 0 ; --iStart, ++iDelta)
    {
        if(strncmp(prefix, sufix + iDelta, iMaxSize - iDelta) == 0)
        {
            
            char *tmp = strstr((S + (iMaxSize - iDelta)) , (sufix + iDelta));
            
            if(tmp)
            {
                int lenTmp = strlen(tmp);
                if(lenTmp - (iMaxSize - iDelta) >= (iMaxSize - iDelta))
                {
                        iRet = iMaxSize - iDelta;
                        break;
                }
            }
        }
    }
    free(prefix);
    free(sufix);
    
    return iRet;
}

#include<string>

int solution(std::string &S) 
{
    int iRet = 0;
    int ilen = S.length();
    
    if(ilen < 3)
        return 0;
   
    int iMaxLen = ilen / 3;
    int iDelta = 0;
    
    do
    {
        if(S.compare(0, iMaxLen - iDelta, S, ilen - iMaxLen + iDelta, iMaxLen - iDelta) == 0)
        {
            //std::cout<<"FOUND: "<<S.find(S.substr(0, iMaxLen - iDelta),iMaxLen - iDelta, ilen - iMaxLen + iDelta)<<std::endl;
            
            std::size_t found = S.find(S.substr(0, iMaxLen - iDelta), iMaxLen - iDelta);
            if(found != std::string::npos && found + (iMaxLen - iDelta) <= ilen - iMaxLen + iDelta)
            {
                iRet = iMaxLen - iDelta;
                break;
            }
                std::cout<<"ASA: "<<S.substr(0, iMaxLen - iDelta)<<std::endl;
                std::cout<<"found: "<<found<<std::endl;
                std::cout<<"found 2: "<<found + (iMaxLen - iDelta)<<" | "<<(ilen - iMaxLen + iDelta)<<std::endl;
        }
        iDelta++;
    }while(iMaxLen - iDelta >= 0);
    
    return iRet;
}

int main()
{
    //char A[] = "bassabsassassb";
    
    std::string As = "baaab";
    
    std::cout<<"RETURn:" <<solution(As);
    
    return 0;
}


