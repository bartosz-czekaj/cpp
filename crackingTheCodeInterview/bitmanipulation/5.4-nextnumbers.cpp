#include <iostream>
#include <tuple>
#include <vector>

int getNext(int number)
{
    auto zeros = 0;
    auto ones = 0;
    auto all = 0;

    auto numberTmp = number;

    while((number & 1) == 0 && number)
    {
        ++zeros;
        ++all;    
        number >>= 1;    
    }

    while((number & 1) == 1)
    {
        ++ones;
        ++all;  
        number >>= 1;
    }

    if(ones + zeros == 31 || ones + zeros == 0)
        return -1;

    numberTmp |= (1 << all);
    numberTmp &= ~((1 << all) - 1);
    numberTmp |= (1 << (ones - 1 )) - 1;
    
    return numberTmp;
}

int getPrev(int number)
{
    auto zeros = 0;
    auto ones = 0;
    auto all = 0;

    auto numberTmp = number;

    while((number & 1)==1)
    {
        ++ones;
        ++all;  
        number >>= 1;
    }

    if (number == 0)
    {
        return -1;
    }

    while((number & 1) == 0 && number)
    {
        ++zeros;
        ++all;    
        number >>= 1;    
    }


    numberTmp &= ((~0) << (all + 1));

    auto mask = (1 << (ones + 1)) - 1;

    numberTmp |= (mask << (zeros - 1));

    return numberTmp;
}

template<typename ... predicates>
auto calculate(predicates ...ps)
{
   return std::make_tuple(ps ...);
}

template<typename ... predicates>
auto calculateFun(predicates ...ps)
{
   return [=](const auto &nb)
   {
        return std::make_tuple(ps(nb) ...);
   };
}

struct testData
{
    int testNb;
    int max;
    int min;
};

void test(std::vector<testData> testCases)
{
     const auto &fun = calculateFun(getNext, getPrev);

     for(const auto &tCase : testCases)
     {
         const auto [max, min] = fun(tCase.testNb);
         if(max != tCase.max || min != tCase.min)
         {
             std::cout<<"ERROR for nb ["<<tCase.testNb<<" expected [max, min] ["<<tCase.max<<", "<<tCase.min<<"] given ["<<max<<", "<<min<<"] \n";
         }
     }
}


int main()
{
    std::vector<testData> tCases
    {
        {5, 6, 3},
        {8, 16, 4}
    };
    
    test(tCases);
    return 0;
}