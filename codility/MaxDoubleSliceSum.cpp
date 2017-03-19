// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <algorithm>
#include <vector>

int solution(std::vector<int> &A)
{
    std::vector<int> vSumFromBegin;
    std::vector<int> vSumFromEnd;

    vSumFromBegin.resize(A.size());
    vSumFromEnd.resize(A.size());

    int vMaxSumB = 0;
    int vMaxSumE = 0;

    int vIdxStart = A.size() - 2;

    for(int vIdx = 0; vIdx < A.size(); ++vIdx)
    {
        if (vIdx > 0 && vIdx < A.size() - 1)
        {
            vMaxSumB = std::max(0, A.at(vIdx) + vMaxSumB);
            vSumFromBegin[vIdx] = (vMaxSumB);
        }

        if(vIdxStart < A.size() - 1 && vIdxStart > 0)
        {
            vMaxSumE = std::max(0, A.at(vIdxStart) + vMaxSumE);
            vSumFromEnd[vIdxStart] = vMaxSumE;

            --vIdxStart;
        }
    }

    int vMaxDoubleSlice = 0;

    for (int vIdx = 0; vIdx < A.size() - 2; ++vIdx) {
        vMaxDoubleSlice = std::max(vMaxDoubleSlice, vSumFromBegin[vIdx] + vSumFromEnd[vIdx + 2]);
    }

    return vMaxDoubleSlice;
}