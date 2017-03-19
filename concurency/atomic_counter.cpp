#include <thread>
#include <iostream>
#include <atomic>
#include <vector>

const int ITERS = 10000000;

std::atomic<int> counter;

void increment()
{
	for (int i = 0; i < ITERS; ++i)
	{
		counter.fetch_add(1,std::memory_order_relaxed);
	}
}

void decrement()
{
	for (int i = 0; i < ITERS; ++i)
	{
		counter.fetch_sub(1, std::memory_order_relaxed);
	}
}

int main(int argc, char* argv[])
{
	std::vector<std::thread> vt;

	std::cout << "The counter is " << counter << std::endl;

	const int nbThreads = std::thread::hardware_concurrency() ? (std::thread::hardware_concurrency() * 2) : 20;

	for (unsigned int i = 0; i < nbThreads; i++)
	{
		if (i % 2 == 0)
			vt.push_back(std::thread(increment));
		else
			vt.push_back(std::thread(decrement));
	}

	for (std::thread &t : vt)
		t.join();

	std::cout << "The counter is " << counter << std::endl;;

	return 0;
}