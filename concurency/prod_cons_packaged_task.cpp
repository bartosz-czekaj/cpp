#include <chrono>
#include <iostream>
#include <future>
#include <thread>



int myComputation(int x)
{
    for (unsigned i = 0; i < 999999999; ++i)
    {
        x++;
    }
    return x;
}

int main() 
{
    // Create promises
    std::packaged_task<int(int)> task1(&myComputation);
    std::packaged_task<int(int)> task2(&myComputation);

    // Get futures
    std::future<int> val1 = task1.get_future();
    std::future<int> val2 = task2.get_future();

    // Schedule promises
    std::thread t1(move(task1), 0);
    std::thread t2(move(task2), 0);

    // Print status while we wait
    bool s1 = false, s2 = false;
    do {
        s1 = val1.wait_for(std::chrono::milliseconds(50)) == std::future_status::ready;
        s2 = val2.wait_for(std::chrono::milliseconds(50)) == std::future_status::ready;

        std::cout << "Value 1 is " << (s1 ? "" : "not ") << "ready" << std::endl;
        std::cout << "Value 2 is " << (s2 ? "" : "not ") << "ready" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    } while (!s1 || !s2);

    // Cleanup threads-- we could obviously block and wait for our threads to finish if we don't want to print status.
    t1.join();
    t2.join();

    // Final result
    std::cout << "Value 1: " << val1.get() << std::endl;
    std::cout << "Value 2: " << val2.get() << std::endl;
    return 0;
}