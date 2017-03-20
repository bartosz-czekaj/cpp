#include <iostream>
#include <thread>

class DisplayThread
{
public:
    void operator()()
    {
        for (int i = 0; i < 100; ++i)
            std::cout << "Display Thread Executing: "<< (i + 1) << std::endl;
    }
};

int main()
{
    std::thread threadObj((DisplayThread()));
    for (int i = 0; i < 100; ++i)
    {
        std::cout << "Display From Main Thread: "<< (i+1) << std::endl;
    }
    std::cout << "Waiting For Thread to complete" << std::endl;
    threadObj.join();
    std::cout << "Exiting from Main Thread" << std::endl;
    return 0;
}