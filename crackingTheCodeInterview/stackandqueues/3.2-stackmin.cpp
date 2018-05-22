#include <stack>
#include <iostream>

class stackMin
{
    std::stack<int> data;
    std::stack<int> min;
public:
    void push(int x)
    {
        data.push(x);
        if(min.empty())
        {
            min.push(x);
        }
        else
        {
            if(x <= min.top())
            {
                min.push(x);
            }
        }
    }

    void pop()
    {
       if(data.top() == min.top())
       {
           min.pop();
       }
       data.pop();
    }

    void clear()
    {
        auto cc = [](auto &stack)
        {
            while(!stack.empty())
            {
                stack.pop();
            }
        };
        cc(data);
        cc(min);
    }

    auto minVal()
    {
        return min.top();
    }
};


void test(stackMin &sm)
{
    sm.push(1);

    if(sm.minVal() != 1)
    {
        std::cout<<"Error min != "<<1<<'\n';
    }

    sm.push(2);
    sm.push(2);
    sm.push(3);

    if(sm.minVal() != 1)
    {
        std::cout<<"Error min != "<<1<<'\n';
    }

    sm.clear();

    for(int x = 10 ; x >= 1 ; --x)
    {
        sm.push(x);
        if(sm.minVal() != x)
        {
            std::cout<<"Error min != "<<x<<'\n';
        }
    }

    for(int x = 1 ; x<= 10 ; ++x)
    {
        if(sm.minVal() != x)
        {
            std::cout<<"Error min != "<<x<<'\n';
        }
        sm.pop();
    }
}

int main()
{
    stackMin sm;
    test(sm);
    return 0;
}