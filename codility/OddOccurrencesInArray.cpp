// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    int vRet = 0;

    for (auto &vValue : A)
    {
        vRet ^= vValue;
    }

    return vRet;
}