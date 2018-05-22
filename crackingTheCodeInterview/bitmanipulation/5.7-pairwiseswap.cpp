#include <iostream>
#include <vector>
#include <limits>
#include <type_traits>
#include <stdexcept>

template<typename T>
T pairwiseSwap(T number)
{
    if constexpr (std::is_integral_v<T>)
        return ((number & 0xAAAAAAAA) >> 1) | ((number & 0x55555555) << 1);
    else
        throw std::runtime_error("Integral type needed!\n");
}

template<typename T>
void test(const std::vector<T> &nbrs)
{

    if constexpr (std::is_integral_v<T>)
    {
        for(const auto &nb : nbrs)
        {
            auto tmp = pairwiseSwap(nb);
            auto tmp2 = pairwiseSwap(tmp);
            if(tmp2 !=  nb)
            {
                std::cout<<"Error  expected: "<<nb<<" given: "<<tmp2<<'\n';
            }
        }
    }
    else
        throw std::runtime_error("Integral type needed!\n");
}


int main()
{
    std::vector<unsigned> nbrs1 {1,0, 5, 8, 16, 17,std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::min()};
    std::vector<int> nbrs2 {-1,0, 5, 8, 16, 17,std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    std::vector<double> nbrs3 {std::numeric_limits<double>::max(), std::numeric_limits<double>::min()};
    test(nbrs1);
    test(nbrs2);

    try
    {
        test(nbrs3);
    }
    catch(const std::exception &e)
    {
        std::cout<<e.what();
    }

    return 0;
}