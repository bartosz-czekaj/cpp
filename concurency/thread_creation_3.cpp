#include <iostream>
#include <thread>

int main()
{
    std::thread threadObj([] {
        for (int i = 0; i < 100; ++i)
        {
            std::cout << "Display Thread Executing" << (i + 1) << std::endl;
        }
    });

    for (int i = 0; i < 100; ++i)
    {
        std::cout << "Display From Main Thread" <<(i+1)<< std::endl;
    }
    threadObj.join();
    std::cout << "Exiting from Main Thread" << std::endl;
    return 0;
}