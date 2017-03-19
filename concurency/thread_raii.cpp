#include <iostream>
#include <thread>

class ThreadRAII
{
    std::thread m_thread;
public:
    ThreadRAII(std::thread &&threadObj) : m_thread(std::move(threadObj))
    {

    }
    ~ThreadRAII()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
};
void thread_function()
{
    for (int i = 0; i < 10000; i++)
        std::cout << "thread_function Executing" << std::endl;
}

int main()
{
    std::thread threadObj(thread_function);

    ThreadRAII wrapperObj(std::move(threadObj));

    if (threadObj.joinable())
    {
        threadObj.join();
    }

    return 0;
}