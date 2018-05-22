#include <vector>
#include <algorithm>
#include <iostream>

template <typename Predicate, typename Action>
auto if_then(Predicate predicate, Action action)
{
    return[=](auto&& x)
    {
        if(predicate(x))
        {
            action(std::forward<decltype(x)>(x));
        }
    };
}

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

auto multipleIt(int m = 2)
{
    return [=](const auto&  x)
    {
        std::cout<<x<<" "<<(x*m)<<'\n';
        return x * m;
    };
}

int main()
{
    std::vector<int> nbrs;
    for(int i = 1; i< 1000; ++i)
    {
        nbrs.emplace_back(i);
    }

    std::for_each(nbrs.begin(), nbrs.end(), if_then(
                                                when_all(isModulo(12), isModulo(46), isGreaterThan(300)),
                                                multipleIt(5)));
    

    return 0;
}