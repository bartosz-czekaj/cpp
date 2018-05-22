#include <iostream>
#include <string>
#include <limits>

std::string printBinary(double num)
{
    if(num >= 1 || num <= 0)
    {
        return "error";
    }
    std::string binaryStr {"."};
    const auto epsilon = std::numeric_limits<double>::epsilon();
    
    while(epsilon < num)
    {
        if(binaryStr.length() > 32)
        {
            return "error";
        }

        auto rest = num * 2;
        if(rest >= 1)
        {
            binaryStr.push_back('1');
            num = rest - 1;
        }
        else
        {
            binaryStr.push_back('0');
            num = rest;
        }
    }

    return binaryStr;
}

int main()
{
    std::cout << printBinary(0.17)<<'\n';
    return 0;
}