#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

static std::mutex theLock;

void produce(int &data) 
{
    for (int i = 1; i <= 5; ++i) 
    {
        {
            std::lock_guard<std::mutex> lock(theLock);
            data = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    int data = 0;
    int val = 0;

    // Start thread
    std::thread t(produce, std::ref(data));

    // Loop through the data until
    do {
        {
            std::lock_guard<std::mutex> lock(theLock);
            if (data != 5) 
            {
                data += 6;
            }
            val = data;
        }
        std::cout << "My current value " << val << std::endl;
    }while (val != 5);

    // Cleanup
    t.join();

    return 0;
}