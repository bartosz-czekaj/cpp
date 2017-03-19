#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <deque>


class ThreadGuard
{
	std::thread &_thread;
public:
	explicit ThreadGuard(std::thread &thread)
		:_thread(thread){}

	~ThreadGuard()
	{
		if (_thread.joinable())
		{
			_thread.join();
		}
	}

	ThreadGuard(ThreadGuard const&) = delete;
	ThreadGuard& operator=(ThreadGuard const&) = delete;

};

std::mutex mu;

void fun()
{
	std::cout << "From Thread";
}

int main()
{
	std::thread t(fun);
	ThreadGuard g(t);


	return 0;
}