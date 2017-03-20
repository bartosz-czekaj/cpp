#include <iostream>
#include <thread>

void thread_function()
{
    for (int i = 0; i < 100; ++i)
    {
        std::cout << "thread function Executing: "<<(i+1) << std::endl;
    }
}

int main()
{
    std::thread threadObj(thread_function);
    for (int i = 0; i < 100; ++i)
    {
        std::cout << "Display From MainThread: "<< (i + 1) << std::endl;
    }
    threadObj.join();
    std::cout << "Exit of Main function" << std::endl;
    return 0;
}