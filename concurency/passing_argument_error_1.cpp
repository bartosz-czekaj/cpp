#include <string>
#include <thread>
#include <iostream>

void newThreadCallback(int * p)
{
    std::cout << "Inside Thread :  "" : p = " << *p << std::endl;
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);
    *p = 19;//writes memory
}
void startNewThread()
{
    int i = 10;
    std::cout << "Inside Main Thread :  "" : i = " << i << std::endl;
    std::thread t(newThreadCallback, &i);
    t.detach();
    std::cout << "Inside Main Thread :  "" : i = " << i << std::endl;
}
int main()
{
    startNewThread();
    std::chrono::milliseconds dura(2000);
    std::this_thread::sleep_for(dura);
    return 0;
}