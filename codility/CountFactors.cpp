// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <set>
#include <algorithm>
int solution(int A)
{
    std::set<int> vFactors;

    int vTreshold = std::ceil(std::sqrt(A));
    int vStart = 1;

    if ((A % vTreshold) == 0)
        vFactors.insert(vTreshold);

    while(vStart < vTreshold)
    {
        if ((A % vStart) == 0)
        {
            vFactors.insert(vStart);
            vFactors.insert(A / vStart);
        }

        ++vStart;
    }

    return vFactors.size();
}