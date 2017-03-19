// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <vector>

int solution(std::vector<int> &A)
{
    constexpr int kUp       = 0;
    constexpr int kDown     = 1;
    constexpr int kStop     = 1000000000;

    long long  vRet = 0;
    bool    vStartCounting = false;
    bool    vDoCount       = false;
    long long     vSumUp       = 0;
    long long     vSumDown = 0;

    for(auto &vElem : A)
    {
        if(vElem == kUp)
        {
            vStartCounting  = true;
            vDoCount = false;

            vRet += vSumDown * vSumUp;

            if(kStop < vRet)
            {
                vRet = -1;
                break;
            }

            vSumDown = 0;
            ++vSumUp;
        }
        else
        {
            if(vStartCounting)
            {
                vDoCount = true;
                ++vSumDown;
            }
        }
    }

    if (vDoCount)
    {
        vRet += vSumDown * vSumUp;
        if (kStop < vRet)
            vRet = -1;
    }

    return static_cast<int>(vRet < -1 ? -1 : vRet);
}