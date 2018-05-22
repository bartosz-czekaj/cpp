#include <iostream>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <variant>

template  <typename ... T>
class overload : T ...
{
    public:
    overload(T ... t) : T(t)...
    {

    }
    using T::operator ()...;
};


int main()
{
    std::vector<std::variant<char, long, float, int, double, long long>>      // 1
             vecVariant = {5, '2', 5.4, 100ll, 2011l, 3.5f, 2017};
    auto ov = overload(
        [=](char t){std::cout<<"char "<<t<<"\n";},
        [=](int t){std::cout<<"int: "<< t <<"\n";},
        [=](long t){std::cout<<"long: "<<t<<"\n";},
        [=](float t){std::cout<<"float: "<<t<<"\n";},
        [=](auto t){std::cout<<"Dont Know: "<<t<<"\n";}
        );

    auto func = [&](auto &t)
    {
        std::visit(ov, t);
    };

    for(auto& v: vecVariant)
    {
        func(v);
    }
    return 0;
}