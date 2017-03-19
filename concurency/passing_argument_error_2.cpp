#include <string>
#include <thread>
#include <iostream>
void newThreadCallback(int * p)
{
    std::cout << "Inside Thread :  "" : p = " << p << std::endl;
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);
    *p = 19;
}
void startNewThread()
{
    int * p = new int();
    *p = 10;
    std::cout << "Inside Main Thread :  "" : *p = " << *p << std::endl;
    std::thread t(newThreadCallback, p);
    t.detach();
    delete p;
    p = nullptr;
}
int main()
{
    startNewThread();
    std::chrono::milliseconds dura(2000);
    std::this_thread::sleep_for(dura);
    return 0;
}