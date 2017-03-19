#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::timed_mutex _timed_mutex;

void doWork()
{
	for (int i = 0; i < 100000; ++i)
	{
		std::unique_lock<std::timed_mutex> lock(_timed_mutex, std::defer_lock);//mutex not yet beeing lock

		long long count = 0;
		while (lock.try_lock_for(std::chrono::microseconds(1)) == false)//
		{
			++count;
		}

		std::cout << std::this_thread::get_id() << " count: " << count << std::endl;
	}
}

int main()
{
	std::vector<std::thread> threads;

	const unsigned int THREADS = std::thread::hardware_concurrency() * 2;

	for (unsigned int i = 0; i < THREADS; ++i)
	{
		threads.emplace_back(doWork);
	}

	for (auto & t : threads)
	{
		t.join();
	}

	return 0;
}