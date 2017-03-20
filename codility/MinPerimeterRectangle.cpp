// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
#include <algorithm>
int solution(int A)
{
    int vRest = std::floor(std::sqrt(A));

    int vRet = 0;

    while(1)
    {
        if((A % vRest) == 0)
        {
            vRet = 2 * (vRest + A / vRest);

            break;
        }

        --vRest;

    }

    return vRet;
}