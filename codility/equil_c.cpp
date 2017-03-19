int solution(int A[], int N) 
{
    int iEquiIdx = -1;
    int iIdx = 1;
    long long iSum = 0;
    
    for(iIdx = 0; iIdx < N; ++iIdx)
        iSum += A[iIdx];
    
    iIdx = 1;
    
    long long iSumLeft = A[0];
    
    do{
        if(iSumLeft == iSum - iSumLeft - A[iIdx])
        {
           iEquiIdx = iIdx;
           break;
        }
        iSumLeft += A[iIdx];
        ++iIdx;
        
    }while(iIdx < N -1);
    
    return iEquiIdx;
}