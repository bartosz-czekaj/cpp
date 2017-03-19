// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <algorithm>
#include <vector>

int solution(std::vector<int> &A)
{
    if (A.size() <= 1)
        return 0;

    int vMaxEndingHere = 0;
    int vMaxEndingSoFar = 0;
    int vMin = A.at(0);

    for (int vIdx = 1; vIdx < A.size(); ++vIdx)
    {
        if (A.at(vIdx) < vMin)
            vMin = A.at(vIdx);

        vMaxEndingHere = std::max(0, A.at(vIdx) - vMin);
        vMaxEndingSoFar = std::max(vMaxEndingHere, vMaxEndingSoFar);
    }

    return vMaxEndingSoFar;
}