#include <condition_variable>
#include <mutex>
#include <future>
#include <iostream>
#include <chrono>
#include <queue>

std::mutex mutex;
std::condition_variable condvar;

std::queue<int> messageQ;

void Producer(int iHowMany)
{
    std::cout << "=== Producer ===" << std::endl;

    for (auto x = 0; x < iHowMany; ++x)
    {
        {
            std::lock_guard<std::mutex> guard(mutex);
            std::cout << "Producing message: " << x << " th" << std::endl;
            messageQ.push(x);
        }
        condvar.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }


    std::cout << "=== Producer has completed ===" << std::endl;

}

void Consumer()
{
    std::cout << "\n\n=== Consumer is ready to get message ===" << std::endl;

    while (true)
    {
        {
            std::unique_lock<std::mutex> ulock(mutex);
            condvar.wait(ulock, [&]()
            {
                return messageQ.size() > 0;
            });
        }

        while (!messageQ.empty())
        {
            std::lock_guard<std::mutex> guard(mutex);
            int i = messageQ.front();
            std::cout << "Consuming message: " << i << " th" << std::endl;
            messageQ.pop();
        }

        if (!messageQ.empty())
            std::cout << "There are still messages remained from producer" << std::endl;
        else
            std::cout << "All messages from producer has been processed" << std::endl;
    }


}


int main()
{
    auto t1 = std::async(std::launch::async, Producer, 12);
    auto t2 = std::async(std::launch::async, Consumer);

    t1.get();
    t2.get();

    return 0;
}