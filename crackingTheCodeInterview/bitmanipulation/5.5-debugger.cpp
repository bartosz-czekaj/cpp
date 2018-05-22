
#include <iostream>
#include <vector>
#include <exception>

class myexception: public std::exception
{
    public:
    const char* what() const throw() override
    {
        return "negative number";
    }
};

struct testData
{
    int number;
    bool isEven;
};

template<int number>
constexpr bool test()
{
    if constexpr(number >= 0)
        return ((number & (number - 1)) == 0);
    else
        throw myexception();     
}


int main()
{
   std::cout<<test<2>()<<'\n';
   std::cout<<test<3>()<<'\n';
   std::cout<<test<4>()<<'\n';
   std::cout<<test<0>()<<'\n';

   try
   {
       std::cout<<test<-1>()<<'\n';
   }
   catch(const myexception &e)
   {
       std::cout<<e.what()<<'\n';
   }

    return 0;
}