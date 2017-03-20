// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
#include <stack>
int solution(std::vector<int> &H)
{
    std::stack<int> vTops;

    int vRect = 0;

    for (const auto &vTop : H)
    {
        if(!vTops.empty())
        {
            while (!vTops.empty() && vTops.top() > vTop)
                vTops.pop();
        }
        if (!vTops.empty() && vTop == vTops.top())
            continue;

        vTops.push(vTop);
        ++vRect;
    }
    return vRect;
}