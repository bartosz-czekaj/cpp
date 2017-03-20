#include <vector>
#include <algorithm>


//100%

int solution(std::vector<int> &A)
{
    std::vector<std::pair<long long, bool>> vPoints;

    long long vIdx = 0;

    for(const auto &vElement : A)
    {
        vPoints.emplace_back(vIdx - vElement, true);
        vPoints.emplace_back(vIdx + vElement, false);
        ++vIdx;
    }

    std::sort(vPoints.begin(), vPoints.end(), [](const std::pair<long long, bool> &aFirst, const std::pair<long long, bool> &aSecond)->bool
    {
        bool vRet = false;
        
        if (aFirst.first < aSecond.first)
            vRet = true;
        else if (aFirst.first > aSecond.first)
            vRet = false;
        else if (aFirst.first == aSecond.first)
        {
            if (aFirst.second == aSecond.second)
                vRet = false;
            else
                vRet = aFirst.second;
        }
        
        return vRet;
    });

    int vActiveCircles = 0;
    int vIntersections = 0;

    for(const auto &vPoint : vPoints)
    {
        if (vIntersections > 10000000)
            return -1;

        if (vPoint.second)
        {

            vIntersections += vActiveCircles;
            ++vActiveCircles;
        }
        else
            --vActiveCircles;
    }

    return vIntersections;
}

//81%
int solution(std::vector<int> &A)
{
    int         vSum    = 0;
    const int   kSize   = A.size();

    long long vColIdx = 0;
    long long vRowIdx = 1;
    long long vChanges = 0;

    while(1)
    {
        if(vRowIdx > kSize - 1)
        {
            ++vChanges;

            vRowIdx = 1 + vChanges;
            ++vColIdx;
        }

        if (vColIdx >= kSize - 1)
            break;

        const long long kColVal = (long long)A[vColIdx];
        const long long kRowVal = (long long)A[vRowIdx];


        if ((kColVal + vColIdx >= vRowIdx - kRowVal) || (vRowIdx - kRowVal <= kColVal + vColIdx))
            ++vSum;

        if (vSum > 1000000000)
        {
            vSum = -1;
            break;
        }

        ++vRowIdx;
    }

    return vSum;
}