#include <thread>
#include <string>
#include <iostream>
#include <chrono>
#include <future>
#include <vector>

int main()
{

    auto id = std::this_thread::get_id();
    std::cout << "Main thread id: "<< id << std::endl;

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 50; ++i)
    {
        futures.emplace_back(std::async(std::launch::async, [](const std::thread::id &mtid)
        {
            auto tID = std::this_thread::get_id();

            const bool equal = tID == mtid;

            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            std::cout << "This thread id: " << std::this_thread::get_id() << " MT ID:" << mtid << (equal? " [E]" : " [NE]") <<std::endl;
        }, id));
    }

    for (auto &ftr : futures)
    {
        ftr.wait();
    }

    std::cout << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    return 0;
}