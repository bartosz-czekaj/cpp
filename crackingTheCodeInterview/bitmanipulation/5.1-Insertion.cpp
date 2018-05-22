#include <iostream>

int updateBits(int m, int n, int i, int j)
{
    auto allOnes = ~0;
    auto left = allOnes << (j + 1);
    auto right = (1 << i) - 1;

    auto mask = left | right;

    auto nCleared = n & mask;
    auto mShiftted = m << i;

    return nCleared | mShiftted;

}


int main()
{
    return 0;
}