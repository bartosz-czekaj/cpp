#include <vector>

int solution(std::vector<int> &A)
{
    int vRetIdx = 0;
    double vMinAvg = 0;
    bool vNotSet = true;

    for (int vIdx = 0; vIdx < A.size() - 1; ++vIdx)
    {
        double vTmpMinAvg = static_cast<double>(A[vIdx] + A[vIdx + 1]) / 2;

        if (vIdx + 2 <= A.size() - 1)
        {
            vTmpMinAvg = std::min(static_cast<double>(A[vIdx] + A[vIdx + 1] + A[vIdx + 2]) / 3, vTmpMinAvg);
        }

        if(vTmpMinAvg < vMinAvg || vNotSet)
        {
            vNotSet = false;
            vMinAvg = vTmpMinAvg;
            vRetIdx = vIdx;
        }
    }

    return vRetIdx;
}