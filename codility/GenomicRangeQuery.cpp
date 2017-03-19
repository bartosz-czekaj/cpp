// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <vector>
#include <algorithm>
#include <numeric>

std::vector<int> solution(std::string &S, std::vector<int> &P, std::vector<int> &Q)
{
    const int kElems = 4;

    std::vector<int> vRet (P.size(), 0);
    std::vector<std::vector<int>> vRange(S.size(), std::vector<int>(kElems));

    std::vector<int> vChars(kElems, 0);

    int vIdx = 0;

    for(auto &vVector : vRange)
    {
        int vIndex = 3;
        switch (S[vIdx++])
        {
        case 'A':
            vIndex = 0;
            break;
        case 'C':
            vIndex = 1;
            break;
        case 'G':
            vIndex = 2;
            break;
        default:
            vIndex = 3;
            break;
        }

        vChars[vIndex] += 1;

        vVector[0] = vChars[0];
        vVector[1] = vChars[1];
        vVector[2] = vChars[2];
        vVector[3] = vChars[3];
    }

    for (int vJdx = 0; vJdx < P.size(); ++vJdx)
    {
        int vItem = 0;
        bool vAdd = true;
        while (vItem < kElems)
        {
            int vBegin = P[vJdx] == 0 ? 0 : vRange[P[vJdx]-1][vItem];
            if(vBegin != vRange[Q[vJdx]][vItem])
            {
                vAdd = false;
                vRet[vJdx] = vItem + 1;
                break;
            }

            ++vItem;
        }
        if(vAdd)
            vRet[vJdx] = vItem;
    }

    return vRet;
}