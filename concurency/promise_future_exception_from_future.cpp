#include <thread>
#include <string>
#include <iostream>
#include <chrono>
#include <future>

#include <thread>
#include <string>
#include <iostream>
#include <chrono>
#include <future>

void thFun(std::promise<std::string>  &prms)
{
    try 
    {
        std::string str("hello from future!");

        throw(std::exception("Exception from future!"));

        prms.set_value(str);
    }
    catch (...)
    {
        prms.set_exception(std::current_exception());
    }

}

int main()
{
    std::promise<std::string>  prms;

    std::thread th(&thFun, std::ref(prms));
    std::cout << "Hello from main!\n";

    std::future<std::string>  ftr = prms.get_future();
    try
    {
        std::string str = ftr.get();
        std::cout << str << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    th.join();

    return 0;
}