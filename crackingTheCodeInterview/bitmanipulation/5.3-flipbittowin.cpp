#include <iostream>
#include <algorithm>
#include <vector>

int maxlen(int number)
{
    auto previous = 0;
    auto currentmax = 0;
    auto maxLen = 0;

    const auto &calculateMaxlen = [&]()
    {
        maxLen =  std::max(maxLen, currentmax + previous + 1);
    };

    while(number)
    {
        auto currentBit = number & 1;

        if(currentBit)
        {
            ++currentmax;    
        }
        else
        {
            calculateMaxlen();
            previous = currentmax;
            currentmax = 0;
        }


        number =  number >> 1;

    }

    calculateMaxlen();
    return maxLen;
}


struct testData
{
    int number;
    int excpected;
};

void test(const std::vector<testData> &testCases)
{
    for(const auto &testCase : testCases)
    {
        auto given = maxlen(testCase.number);
        if(given != testCase.excpected)
        {
            std::cout<<"Error for number ["<<testCase.number<<"] "<<"["<<testCase.excpected<<"] given: "<<given<<'\n';
        }
    }
}

int main()
{
    std::vector<testData> testCases 
    {
        {237, 6},
        {1775, 8},
        {0, 1},
        {1, 2},
        {9, 2}
    };
    test(testCases);
}