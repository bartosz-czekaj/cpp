// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <vector>
#include <deque>

int solution(std::vector<int> &A)
{
    int vSize = A.size();

    int vSumF = 0;
    int vSumB = 0;

    int vIdxB = vSize - 1;

    int vRet = -1;

    std::deque<int> vSumForward, vSumBackward;

    for (int vIdx = 0; vIdx < vSize - 1; ++vIdx, --vIdxB)
    {
        vSumF += A[vIdx];
        vSumB += A[vIdxB];

        vSumForward.push_back(vSumF);
        vSumBackward.push_front(vSumB);
    }

    for (int vIDX = 0; vIDX < vSumBackward.size(); ++vIDX)
    {
        int vDiff = std::abs(vSumBackward[vIDX] - vSumForward[vIDX]);;
        if (vRet < 0 || vDiff < vRet)
            vRet = vDiff;

        if (vRet == 0)
            break;
    }

    return vRet;

}