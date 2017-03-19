// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <algorithm>

int solution(int A, int B, int K)
{
    int vRet = std::floor(static_cast<double>(B) / static_cast<double>(K)) - std::floor(static_cast<double>(A) / static_cast<double>(K));

    if ((vRet == 0  && A % K == 0 ) || A == 0 || A == K || B == K || A % K == 0)
        ++vRet;

    return vRet;
}