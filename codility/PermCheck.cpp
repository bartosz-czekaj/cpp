// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <vector>
#include <algorithm>
#include <functional>

int solution(std::vector<int> &A)
{
    int vSize = A.size();
    std::sort(std::begin(A), std::end(A));

    auto vLast = std::unique(std::begin(A), std::end(A));
    A.erase(vLast, A.end());

    if (A.size() == vSize && A[0] == 1 && A[A.size() - 1] == A.size())
        return 1;

    return 0;
}