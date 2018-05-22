#include <stack>
#include <iostream>

template <typename T>
class queueViaStack
{
    std::stack<T> newest;
    std::stack<T> oldest;

    void shiftStack()
    {
        if(oldest.empty())
        {
            while(!newest.empty())
            {
                oldest.push(newest.top());
                newest.pop();
            }
        }
    }

public:
    void push(T value)
    {
        newest.push(value);
    }

    T top()
    {
        shiftStack();
        return oldest.top();
    }

    void pop()
    {
        shiftStack();
        oldest.pop();
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

        clear(newest);
        clear(oldest);
    }


};

void test(auto &qvs)
{
    auto msg =[&](auto &&expected)
    {
        if(qvs.top() != expected)
        {
            std::cout<<"Error top: "<<qvs.top()<<" expected "<<expected<<'\n';
        }
    };

    for(int i = 0; i < 5; ++i)
    {
        qvs.push(i);
    }

    msg(0);


    qvs.clear();

    for(int i = 0; i < 15; ++i)
    {
        qvs.push(i);
    }


    for(int i = 0; i < 15; ++i)
    {
        msg(i);
        qvs.pop();
    }

    qvs.clear();

    for(int i = 0; i < 15; ++i)
    {
        qvs.push(i);
    }
    for(int i = 0; i < 12; ++i)
    {
        qvs.pop();
    }

    msg(12);
}

int main()
{
    queueViaStack<int> qvs;
    test(qvs);
    return 0;
}