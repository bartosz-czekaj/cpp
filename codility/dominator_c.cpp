int solution(int A[], int N) 
{
    int iMaxProfit = 0;
    int iIdx = 0;
    
    int max_diff = A[1] - A[0];
    int min_element = A[0];
    
    for(iIdx = 1; iIdx < N; ++iIdx)
    {       
        if(A[iIdx] - min_element > max_diff)                               
          max_diff = A[iIdx] - min_element;
        if(A[iIdx] < min_element)
          min_element = A[iIdx];                     
    }
    
    if(max_diff > 0)
        iMaxProfit = max_diff;

    return iMaxProfit;
}