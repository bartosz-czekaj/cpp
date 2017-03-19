#include <vector>
#include <algorithm>

int solution(std::vector<int> &A)
{
    int vRet = 0;

    if (A.size() < 3)
        return vRet;

    std::sort(A.begin(), A.end());

    for (int vIdx = 0; vIdx < A.size() - 2; ++vIdx)
    {
        if(A[vIdx] > A[vIdx + 2] - A[vIdx + 1])
        {
            vRet = 1;
            break;
        }
    }

    return vRet;
}