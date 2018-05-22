#include <stack>
#include <iostream>

template <typename T>
class SortedStack
{
    std::stack<T> sorted;
    std::stack<T> helper;
public:
    void push(T value)
    {
        T tmpVal = value;

        while(!sorted.empty() && sorted.top()<tmpVal)
        {
            helper.push(sorted.top());
            sorted.pop();
        }

        sorted.push(tmpVal);

        while(!helper.empty())
        {
            sorted.push(helper.top());
            helper.pop();
        }
    }

    void pop()
    {
        sorted.pop();
    }

    T top()
    {
        return sorted.top();
    }

    void clear()
    {
        auto clear = [](std::stack<T> &stack)
        {
            while(!stack.empty())
            {
                stack.pop();
            }
        };

        clear(sorted);
        clear(helper);
    }
};

void test(auto &&sstack)
{
    auto msg =[&](auto &&expected)
    {
        if(sstack.top() != expected)
        {
            std::cout<<"Error top: "<<sstack.top()<<" expected "<<expected<<'\n';
        }
    };

    for(auto i = 100; i >=1; --i)
    {
        sstack.push(i);
        msg(i);
    }

    sstack.clear();

    for(auto i = 1; i <= 10; ++i)
    {
        sstack.push(i);
    }

    msg(1);

    for(auto i = 1; i <= 9; ++i)
    {
        sstack.pop();
    }

    msg(10);

    for(auto i = 19; i >= 11; --i)
    {
        sstack.push(i);
    }

    msg(10);

    for(auto i = 9; i >= 1; --i)
    {
        sstack.push(i);
        msg(i);
    }

}

int main()
{
    test(SortedStack<int>());

    return 0;
}