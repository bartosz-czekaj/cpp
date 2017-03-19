int solution(int A[], int N)
{
int iIndex = 1;
int iAbsolute = 1;

while(iIndex < N)
{
int j = 0;
int ile = 1;
 
for(j = iIndex - 1; j >= 0; --j)
{
  if(abs(A[iIndex]) ==  abs(A[j]))
  {
    ile = 0;
    break;
  }
  if(abs(A[iIndex]) <  abs(A[j]))
  break;
}
 
  iAbsolute += ile;       
  ++iIndex;
}
 
return iAbsolute;
}

#include <stdlib.h>     /* calloc, exit, free */

int solution(int A[], int N) 
{
    int iSize = abs(A[0]) > abs(A[N - 1]) ? abs(A[0]) : abs(A[N - 1]);
    int iAbsolute = 0;
    
    int *tmp = 0;
    tmp = (int*) calloc (iSize + 1,sizeof(int));
    int i = 0;
    
    for(i = 0; i < N; ++i)
    {
        tmp[abs(A[i])] = tmp[abs(A[i])] + 1;
    }
    
    for(i = 0; i < iSize + 1; ++i)
    {
       if (tmp[i] > 0)
       {
           ++iAbsolute;
       }
    }
    
    
   free(tmp);
    
    return iAbsolute;
}

#include <stdio.h>
#include <stdlib.h>

int comp (const void * elem1, const void * elem2) 
{
    int f = abs(*((int*)elem1));
    int s = abs(*((int*)elem2));
    
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int solution(int A[], int N) 
{
    int iAbsolute = 1;
    int i = 0;
    
    qsort (A, N, sizeof(int), comp);

    int iPrevVal = abs(A[0]);    
    
    for(i = 1; i < N; ++i )
    {
        if(iPrevVal != abs(A[i]))
        {
                ++iAbsolute;
                iPrevVal = abs(A[i]);
        }
    }
    
    return iAbsolute;
}