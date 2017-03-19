// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <algorithm>
#include <set>

int solution(int X, std::vector<int> &A)
{
    int vRet    = -1;
    int vIdx = 0;

    std::set<int> vTmpSet(A.begin(), X < A.size() ? A.begin() + X : A.end());

    if (vTmpSet.size() == X)
        vRet = X - 1;
    else
    {
        auto vStartItr = A.end();

        if (X + 1 < A.size())
        {
            vIdx = X;

            vStartItr = A.begin() + X;
            for (auto vItr = vStartItr; vItr < A.end(); ++vItr, ++vIdx)
            {
                vTmpSet.insert(*vItr);

                if (vTmpSet.size() == X)
                {
                    vRet = vIdx;
                    break;
                }
            }
        }
    }

    return vRet;
}