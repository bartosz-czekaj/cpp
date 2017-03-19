#include "stdafx.h"
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <string>

template<typename T>
struct MonitorParellelAccumulate
{
public:
	MonitorParellelAccumulate(const T &res) :
		result(res)
	{}

	void accumulate(T && res)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		result += res;

	}

	T getaccumulator()
	{
		return result;
	}

private:
	std::mutex _mutex;
	T result;
};

template<typename Iterator, typename T>
struct accumulate_block
{
	void operator()(Iterator firts, Iterator last, MonitorParellelAccumulate<T>& accumulator)
	{
		T result = T();

		accumulator.accumulate (std::move(std::accumulate(firts, last, result)));
	}
};



template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	const unsigned long LEN = std::distance(first, last);

	if (!LEN)
		return init;

	const unsigned long min_per_thread = 25;
	const unsigned long max_threads = (LEN + min_per_thread - 1) / min_per_thread;

	const unsigned long hardware_threads = std::thread::hardware_concurrency();

	const unsigned long num_threads = std::min((hardware_threads != 0 ? hardware_threads : 2), max_threads);

	const unsigned long block_size = LEN / num_threads;

	MonitorParellelAccumulate<T> monitor(init);

	std::vector<std::thread> threads(num_threads);

	Iterator block_start = first;
	for (unsigned long i = 0; i < num_threads; ++i)
	{
		const bool isLast = i == num_threads;

		Iterator block_end = (isLast ? last : block_start);

		if(!isLast)
			std::advance(block_end, block_size);

		threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(monitor));

		if(!isLast)
			block_start = block_end;
	}

	for (auto &th : threads)
	{
		th.join();
	}

	return monitor.getaccumulator();
}

int main()
{

	std::vector < std::string > vec;
	for (int i = 0; i < 10000; ++i)
	{
		vec.emplace_back(std::to_string(i + 1));
	}

	std::string res = "0";

	{
		auto startTime = std::chrono::system_clock::now();
		std::string  res2 = parallel_accumulate(vec.begin(), vec.end(), res);
		auto stopTime = std::chrono::system_clock::now();

		std::cout << "Parellel accumulate: "  <<res2.length()<< " Execution time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime)).count() << std::endl;;
	}

	{
		auto startTime = std::chrono::system_clock::now();
		std::string res2 = std::accumulate(vec.begin(), vec.end(), res);
		auto stopTime = std::chrono::system_clock::now();

		std::cout << "std::accumulate: " << res2.length() << " Execution time: " << (std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime)).count();
	}

	

	return 0;
}