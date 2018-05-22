#include <iostream>
#include <vector>

int conversion(int numA, int numB)
{
    auto num = numA ^ numB;
    auto elem = 0;

    while(num)
    {
        if(num & 1)
        {
            ++elem;
        }

        num >>= 1;
    }

    return elem;
}

int conversion2(int numA, int numB)
{
    auto elem = 0;

    for(auto num = numA ^ numB; num != 0; num = num & (num -1))
    {
        ++elem;
    }

    return elem;
}

struct testData
{
    int numA;
    int numB;
    int res;
};


void test(const std::vector<testData> &data)
{

    const auto &f = [](auto &res, auto &expected){

        if(res != expected)
        {
            std::cout<<"ERROR expected: "<<expected<<" given: "<<res<<'\n';
        }
    };


    for(const auto &elem : data)
    {
        auto elem1 = conversion(elem.numA, elem.numB);
        auto elem2 = conversion2(elem.numA, elem.numB);

        f(elem1, elem.res);
        f(elem2, elem.res);

    }
}

int main()
{
    std::vector<testData> data
    {
        {29, 15, 2},
        {2, 1, 2},
        {8, 7, 4},
        {5, 3, 2},
        {5, 2, 3}
    };

    test(data);
    
    return 0;
}