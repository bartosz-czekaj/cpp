#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>

struct testData
{
    std::string data1;
    std::string data2;
    bool expected;
};

bool isPermutation(std::string_view str1, std::string_view str2)
{
    if(str1.size() != str2.size())
       return false;

    std::unordered_map<char, int>  checker;

    for(auto idx = 0; idx < str1.size(); ++idx)
    {
        checker[str1[idx]]++;
        checker[str2[idx]]--;
    }

    for(const auto& [k, v] : checker)
    {
        if(v != 0)
            return false;
    }

    return true;
}

bool isPermutation2(std::string_view str1, std::string_view str2)
{
    if(str1.size() != str2.size())
       return false;

    auto usage = 0;

    auto checkMask = [&](const auto& mask)
    {
        if(usage & mask)
        {
            usage &= ~mask;
        }
        else
        {
           usage |= mask;     
        }
    };

    for(auto idx = 0; idx < str1.size(); ++idx)
    {
        auto val_1 = (1 << static_cast<int>(str1[idx])-'a');
        auto val_2 = (1 << static_cast<int>(str2[idx])-'a');

        checkMask(val_1);
        checkMask(val_2);
    }

    return usage == 0;
}

void test(std::vector<testData> &&td)
{
    for(auto& d : td)
    {
        if(isPermutation2(d.data1, d.data2) != d.expected)       
        {
            std::cout<<"ERROR: "<<d.data1<<' '<<d.data2<<'\n';
        }
    }
}


int main()
{
     test(std::vector<testData>{
        {"ala","aal", true},
        {"kolo", "loko", true},
        {"aaa", "aaaa", false},
        {"aaa", "aac", false},
        {"aaapppp", "aaaappp", false},
        {"abcddddrrrooooppppp", "abcddddrrrooooppppp", true}
    });

    return 0;
}