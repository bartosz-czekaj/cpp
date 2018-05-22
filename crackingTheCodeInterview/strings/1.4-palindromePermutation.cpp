#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>

struct testData
{
    std::string data;
    bool expected;
};

bool palindromePermutation(std::string_view strenter)
{
    auto res = 0;
    auto strsize = 0;
    for(const auto &ch : strenter)
    {
        if(ch == ' ')
        {
            continue;
        }

        auto mask = (1<< static_cast<int>(ch)-'a');

        if(mask & res)
        {
            res &= ~mask;
        }
        else
        {
            res |= mask;
        }

        ++strsize;
    }


    return strsize%2 ? (res & (res -1 )) == 0 : res == 0;
}

void test(std::vector<testData> &&td)
{
    for(auto& d : td)
    {
        if(palindromePermutation(d.data) != d.expected)       
        {
            std::cout<<"ERROR: "<<d.data<<'\n';
        }
    }
}


int main()
{
     test(std::vector<testData>{
        {"ako oka", true},
        {"akol oka", true},
        {"akol okal", true},
        {"akol okral", true},
        {"aaaaa", true},
        {"abbab", true},
        {"abbaabbccb", false}
    });

    return 0;
}