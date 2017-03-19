// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <algorithm>
#include <vector>

int solution(std::vector<int> &A)
{
    int vAns = A[0];
    int vSum = 0;

    bool vFirst = true;

    for(const auto &vVal : A)
    {
        if (vFirst)
        {
            vAns = vSum = vVal;
        }
        else
        {
            vSum = std::max(vVal, vSum + vVal);
            vAns = std::max(vSum, vAns);
        }

        vFirst = false;
    }

    return vAns;
}