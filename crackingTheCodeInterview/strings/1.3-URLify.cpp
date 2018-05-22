#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>

struct testData
{
    std::string data;
    std::string expected;
};

std::string urlify(std::string_view strenter)
{
    std::string spaces;
    std::string res;
    
    for(const auto &ch : strenter)
    {
        if(ch == ' ')
            spaces.append("%20");
        else
        {
            res.append(spaces);
            res += ch;
            spaces.clear();
        }    
    }
    
    return res;
}

void test(std::vector<testData> &&td)
{
    for(auto& d : td)
    {
        std::string res = urlify(d.data);
        if(res != d.expected)       
        {
            std::cout<<"ERROR: "<<res<<' '<<d.expected<<'\n';
        }
    }
}


int main()
{
     test(std::vector<testData>{
        {"bez","bez"},
        {" ", ""},
        {"                                         ", ""},
        {"   a                                         ", "%20%20%20a"},
        {"Jon Doe", "Jon%20Doe"},
        {"kotek ", "kotek"},
        {"kacmajor bogdan ", "kacmajor%20bogdan"},
        {"bo   kela ", "bo%20%20%20kela"},
        {"pop ola    ", "pop%20ola"}
    });

    return 0;
}