// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(int N)
{
    int     vBinaryGapSize  = 0;
    int     vRet            = 0;
    bool    vBGStart        = false;

    do
    {
        if ((N & 1) == 0)
            ++vBinaryGapSize;
        else
        {
            if (vBGStart && vBinaryGapSize > vRet)
                vRet = vBinaryGapSize;

            vBinaryGapSize = 0;
            vBGStart = true;
        }

        N >>= 1;
    } while (N);


    return vRet;
}