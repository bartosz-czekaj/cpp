#include <vector>
#include <algorithm>
#include <iostream>

template <typename ... Predicates>
auto when_all(Predicates ...ps)
{
    return [=](const auto& x)
    {
        return (ps(x) && ...);
    };
}

auto isModulo(int modulo = 2)
{
    return [=](const auto&  x){
        return x % modulo == 0;
    };
}

auto isGreaterThan(int minSize = 100)
{
      return [=](const auto&  x){
        return x > minSize;
    };
}

int main()
{
    std::vector<int> nbrs;
    for(int i = 1; i< 1000; ++i)
    {
        nbrs.emplace_back(i);
    }

    auto itr = nbrs.begin();
    while(itr != nbrs.end())
    {
        auto found = std::find_if(itr, nbrs.end(), when_all(isModulo(12), isModulo(46), isGreaterThan(300)));

        if(found != nbrs.end())
        {
            std::cout<<*found<<'\n';
            itr = ++found;
        }
        else 
            break;
    }

    return 0;
}