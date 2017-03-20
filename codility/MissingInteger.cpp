// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <vector>
#include <algorithm>
#include <functional>

int solution(std::vector<int> &A)
{
    std::sort(std::begin(A), std::end(A));

    int vFound = 1;

    if (A.size())
    {
        if (A[A.size() - 1] > vFound)
        {
            auto vItr = std::find_if(std::begin(A), std::end(A), [](int &vArg) {
                return vArg > 0;
            });

            for (auto vIdx = vItr; vIdx != A.end(); ++vIdx)
            {
                if (*vIdx == vFound)
                    ++vFound;

                if (*vIdx > vFound)
                    break;
            }
        }
        else if (A[A.size() - 1] == vFound)
            ++vFound;
    }
    return vFound;
}