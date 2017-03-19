#include <vector>
#include <algorithm>


int solution(std::vector<int> &A)
{
    std::sort(A.begin(), A.end());

    int vIdx = 0;
    int vRet = 1;
    int vRetMinus = 1;

    if (A.size() >= 3)
        vRetMinus = A[0] * A[1];

    for (auto vItr = A.rbegin(); vItr != A.rend(); ++vItr)
    {
        if (vIdx < 1)
            vRetMinus *= *vItr;

        if (vIdx < 3)
            vRet *= *vItr;
        else
            break;
        ++vIdx;
    }

    return std::max(vRet, vRetMinus);
}