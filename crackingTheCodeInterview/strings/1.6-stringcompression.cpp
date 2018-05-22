#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>
#include <cstdlib>
#include <string>

struct testData
{
    std::string data;
    std::string expected;
    int nb;
};

std::string compress(std::string &strToCompress)
{
    std::string strCmprs;
    char prev = '\0';
    auto sum = 0;

    for(const auto &ch : strToCompress)
    {
        if(prev != ch)
        {
            if(sum > 0)
            {
                strCmprs += prev;
                strCmprs += std::to_string(sum);
            }
            sum = 0;
        }

        ++sum;
        prev = ch;
    }

    if(sum > 0)
    {
        strCmprs += prev;
        strCmprs += std::to_string(sum);
    }

    return strCmprs.size() <= strToCompress.size() ? strCmprs : strToCompress;
}

void test(std::vector<testData> &&td)
{
    for(auto& d : td)
    {
        auto strcpr = compress(d.data);
        if((d.nb == 1 && strcpr != d.data) || (d.nb == 2 && strcpr != d.expected))      
        {
            std::cout<<"ERROR: "<<strcpr<<' '<<(d.nb==1 ? d.data : d.expected)<<'\n';
        }
    }
}


int main()
{
     test(std::vector<testData>{
        {"aabcccccaaa","a2b1c5a3",2},
        {"abc","a1b1c1",1},
        {"aaaabc","a4b1c1",2},
        {"baaaabcccccc","b1a4b1c6",2}
    });

    return 0;
}