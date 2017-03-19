#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <vector>
#include <random>

using std::deque;
std::mutex mCoutMu;

class Buffer
{
public:
    void add(int num)
    {
        while (true)
        {
            std::unique_lock<std::mutex> locker(mMu);

            mCondVar.wait(locker, [&]()
            {
                return mBuffer.size() < size_; 
            });

            mBuffer.push_back(num);
            locker.unlock();
            mCondVar.notify_all();
            return;
        }
    }

    int remove()
    {
        while (true)
        {
            std::unique_lock<std::mutex> locker(mMu);

            mCondVar.wait(locker, [&]()
            {
                return mBuffer.size() > 0;
            });

            int back = mBuffer.back();
            mBuffer.pop_back();
            locker.unlock();
            mCondVar.notify_all();
            return back;
        }
    }

    Buffer() {}

private:

    std::mutex mMu;
    std::condition_variable mCondVar;

    deque<int> mBuffer;
    const unsigned int size_ = 10;
};



class Producer
{
public:
    Producer(Buffer &iBuffer)
        : mBuffer(iBuffer)
    {}

    void run() 
    {
        while (true)
        {
            int num = GetRandomVal();

            mBuffer.add(num);
            {
                std::lock_guard<std::mutex> lock(mCoutMu);
                std::cout << "Produced: " << num << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
private:

    int GetRandomVal()
    {
        // Non-deterministic pseudo-random numbers generator
        thread_local std::random_device rd;
        // Pseudo-random engine
        thread_local std::mt19937 engine(rd());
        // Linear distribution in [0, 100]
        thread_local std::uniform_int_distribution<int> dist(0, 99);

        return dist(engine);
    }

    Buffer &mBuffer;
};

class Consumer
{
public:
    Consumer(Buffer &iBuffer)
        : mBuffer(iBuffer)
    {}

    void run()
    {
        while (true)
        {
            int num = mBuffer.remove();
            {
                std::lock_guard<std::mutex> lock(mCoutMu);
                std::cout << "Consumed: " << num << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
private:
    Buffer &mBuffer;
};

int main() 
{
    Buffer vBuffer;
    Producer p(vBuffer);
    Consumer c(vBuffer);

    std::thread producer_thread(&Producer::run, &p);
    std::thread consumer_thread(&Consumer::run, &c);

    producer_thread.join();
    consumer_thread.join();

    getchar();
    return 0;
}