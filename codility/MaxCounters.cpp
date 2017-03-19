// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <vector>
#include <iterator>     // std::back_inserter
#include <vector>       // std::vector
#include <algorithm>    // std::copy

std::vector<int> solution(int N, std::vector<int> &A)
{
    std::vector<int> vRet(N, 0);

    int     vMaxCounter = 0;
    auto    vLastMaxItr = std::find(A.rbegin(), A.rend(), N + 1);
    auto    vMaxIdx     = A.size();
    int     vBase       = 0;
    int     vCurrIdx    = 0;

    if (vLastMaxItr != A.rend())
        vMaxIdx = std::distance(vLastMaxItr, A.rend());

    for(auto &vValue : A)
    {
        if(vValue > vRet.size())
        {

            vBase       = vMaxCounter;

            if (vCurrIdx == (vMaxIdx - 1))
            {
                for (auto &vValue2 : vRet)
                    vValue2 = vMaxCounter;
            }

        }
        else if( vValue > 0 && vValue <= A.size())
        {
            if(vRet[vValue - 1] < vBase)
                vRet[vValue - 1] = vBase;

            vRet[vValue - 1]++;
            if (vMaxCounter < vRet[vValue - 1])
                vMaxCounter = vRet[vValue - 1];
        }

        ++vCurrIdx;
    }

    return vRet;
}