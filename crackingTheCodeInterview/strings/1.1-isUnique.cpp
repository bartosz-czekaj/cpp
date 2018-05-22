#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <algorithm>

struct testData
{
    std::string data;
    bool expected;
};

bool isUnique(std::string &str)
{
    std::sort(str.begin(), str.end());
    for(auto idx = 0; idx < str.size() - 1; ++idx)
    {
        if(str[idx]==str[idx+1])
        {
            return false;
        }
    }
    return true;
}

bool isUniqueConst(std::string_view str)
{
    auto usage = 0;
    for(const auto &ch :str)
    {
        auto val = static_cast<int>(ch)-'a';
        if(usage & (1 << val))
        {
            return false;
        }

        usage |= (1<<val);
    }
    return true;
}

void test(std::vector<testData> &&td)
{
    for(const auto& d : td)
    {
        if(isUniqueConst(d.data) != d.expected)       
        {
            std::cout<<"ERROR: "<<d.data<<'\n';
        }
    }
}


int main()
{
    test(std::vector<testData>{
        {"ala", false},
        {"abcdefghijklmnoprstuvwqxyz", true},
        {"abcdefghijklmnoprstuvwqxyza", false}
    });

    return 0;
}