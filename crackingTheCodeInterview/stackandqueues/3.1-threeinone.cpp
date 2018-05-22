#include <array>
#include <tuple>
#include <iostream>

template<size_t n>
class threeInOne
{
    std::array<int, n> array;
    size_t start1 = 0;
    size_t start2 = n-1;
    size_t start3 = (n-1)/2;

    size_t size1 = 0;
    size_t size2 = 0;
    size_t size3 = 0;

    void Move3rd1()
    {
        if(!IsEmpty3())
        {
             for(auto x = (start3 + size3); x >= start3; --x)
            {
                array[x]=array[x-1];
            }
        }
        ++start3;
    }

    void Move3rd2()
    {
        if(!IsEmpty3())
        {
            for(auto x =start3; x < (start3 + size3); ++x)
            {
                array[x-1]=array[x];
            }
        }
        --start3;
    }

    auto SpaceForMove3rd1()
    {
        return ((start2 - size2 ) - (start3 + size3));
    }

    auto SpaceForMove3rd2()
    {
        return (start3 - (start1+size1));
    }


    bool EnoughtSpace1()
    {
        return size1 + 1 < (start3);
    }

    bool EnoughtSpace2()
    {
        return start2 - size2 > (start3 + (size3-1));
    }

    bool EnoughtSpace3()
    {
        return start3 + size3 <  start2 - (size2-1);
    }

public:
    threeInOne()
    {

    }

    bool push1(int value)
    {
       if(!EnoughtSpace1())
       {
           auto space = SpaceForMove3rd1();
           if(space <= 0)
                return false;
           else
                Move3rd1();     
       }

       array[size1] = value;
       ++size1;

       return true;
    }
    bool push2(int value)
    {
        if(!EnoughtSpace2()) 
        {
           auto space = SpaceForMove3rd2();
           if(space <= 0)
           {
                return false;
           }
           else
                Move3rd2();
        }
        array[start2-size2] = value;
        ++size2;

       return true;
    }
    bool push3(int value)
    {
        if(!EnoughtSpace3()) 
        {
           auto space = SpaceForMove3rd2();
           if(space <= 0)
           {
                return false;
           }
           else
                Move3rd2();     
        }

        array[start3+size3] = value;
        ++size3;

       return true;
    }

    auto pop1()
    {
        auto ret = top1();
        if(std::get<0>(ret) == true)
            --size1;

        return ret;
    }

    auto pop2()
    {
        auto ret = top2();
        if(std::get<0>(ret) == true)
            --size2;

        return ret;
    }

    auto pop3()
    {
        auto ret = top3();
        if(std::get<0>(ret) == true)
            --size3;

        return ret;
    }

    auto top1()
    {
        auto ret = std::make_tuple(false, -1);
        if(IsEmpty1())
            return ret;

        std::get<0>(ret) = true;
        std::get<1>(ret) = array[size1-1];

        return ret;
    }

     auto top2()
    {
        auto ret = std::make_tuple(false, -1);
        if(IsEmpty2())
            return ret;
        std::get<0>(ret) = true;
        std::get<1>(ret) = array[(start2-(size2-1))];

        return ret;
    }

    auto top3()
    {
        auto ret = std::make_tuple(false, -1);
        if(IsEmpty3())
            return ret;
        std::get<0>(ret) = true;
        std::get<1>(ret) = array[(start3+(size3-1))];

        return ret;
    }


    bool IsEmpty1()
    {
        return size1 == 0;
    }
    bool IsEmpty2()
    {
        return size2 == 0;
    }
    bool IsEmpty3()
    {
        return size3 == 0;
    }


};

void test(auto &tio, int elems )
{
    auto  elems_3 = elems/3;

    for(int x = 1; x <= elems_3; ++x)
    {
        tio.push1(x);
        tio.push2(x);
        tio.push3(x);
        auto [exist1, topVal1] = tio.top1();
        auto [exist2, topVal2] = tio.top2();
        auto [exist3, topVal3] = tio.top3();
        if(x != topVal1)
        {
            std::cout<<"ERROR(1): expected="<<x<<" topVal="<<topVal1<<'\n';
        }
        if(x != topVal2)
        {
            std::cout<<"ERROR(2): expected="<<x<<" topVal="<<topVal2<<'\n';
        }
        if(x != topVal3)
        {
            std::cout<<"ERROR(3): expected="<<x<<" topVal="<<topVal3<<'\n';
        }
    }


    for(int x = 1; x <= elems_3; ++x)
    {
         auto [exist1, topVal1] = tio.pop1();
         auto [exist2, topVal2] = tio.pop2();
         auto [exist3, topVal3] = tio.pop3();

         auto expected = elems_3 - (x - 1);

        if(expected != topVal1)
        {
            std::cout<<"ERROR(1): expected="<<expected<<" topVal="<<topVal1<<'\n';
        }
        if(expected != topVal2)
        {
            std::cout<<"ERROR(2): expected="<<expected<<" topVal="<<topVal2<<'\n';
        }
        if(expected != topVal3)
        {
            std::cout<<"ERROR(3): expected="<<expected<<" topVal="<<topVal3<<'\n';
        }
    }

    if(!tio.IsEmpty1())
    {
        std::cout<<"ERROR(1)not empty\n";
    }
    if(!tio.IsEmpty2())
    {
        std::cout<<"ERROR(1)not empty\n";
    }
    if(!tio.IsEmpty3())
    {
        std::cout<<"ERROR(1)not empty\n";
    }

    auto elems_2 = elems/2;
    auto elems_4 = elems/4;
    auto elems_4_12 = elems_4+12;
    for(int x = 1; x <= elems_2; ++x)
    {
        tio.push1(x);
        auto [exist1, topVal1] = tio.top1();
        if(x != topVal1)
        {
            std::cout<<"ERROR(1): expected="<<x<<" topVal="<<topVal1<<'\n';
        }

        if(x > elems_4 && x <= elems_4_12)
        {
            tio.push2(x);
            tio.push3(x);
            auto [exist2, topVal2] = tio.top2();
            auto [exist3, topVal3] = tio.top3();

            if(x != topVal2)
            {
                std::cout<<"ERROR(2): expected="<<x<<" topVal="<<topVal2<<'\n';
            }
            if(x != topVal3)
            {
                std::cout<<"ERROR(3): expected="<<x<<" topVal="<<topVal3<<'\n';
            }
        }
    }


    for(int x = 1; x <= elems_2; ++x)
    {
        auto [exist1, topVal1] = tio.pop1();
       

        auto expected = elems_2 - (x - 1);

        if(expected != topVal1)
        {
            std::cout<<"ERROR(1): expected="<<expected<<" topVal="<<topVal1<<'\n';
        }

        if(x > elems_4 && x <= elems_4_12)
        {
            auto expected = elems_4_12 - (x-(elems_4+1));
            auto [exist2, topVal2] = tio.pop2();
            auto [exist3, topVal3] = tio.pop3();
            if(expected != topVal2)
            {
                std::cout<<"ERROR(2): expected="<<expected<<" topVal="<<topVal2<<'\n';
            }
            if(expected != topVal3)
            {
                std::cout<<"ERROR(3): expected="<<expected<<" topVal="<<topVal3<<'\n';
            }
        }
    }

    if(!tio.IsEmpty1())
    {
        std::cout<<"ERROR(1)not empty\n";
    }
    if(!tio.IsEmpty2())
    {
        std::cout<<"ERROR(1)not empty\n";
    }
    if(!tio.IsEmpty3())
    {
        std::cout<<"ERROR(1)not empty\n";
    }
}

int main()
{
    threeInOne<100> tio;
    threeInOne<1000> tio2;
    threeInOne<300> tio3;
    threeInOne<500> tio4;

    test(tio,100);
    test(tio2,1000);
    test(tio3,300);
    test(tio4,500);
    return 0;
}
