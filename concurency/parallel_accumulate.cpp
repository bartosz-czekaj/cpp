#include "stdafx.h"
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <numeric>

template<typename Iterator, typename T>
struct accumulate_block
{
	void operator()(Iterator firts, Iterator last, T& result)
	{
		result = std::accumulate(firts, last, result);
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

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);

	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}

	accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
	
	for (auto &th : threads)
	{
		th.join();
	}

	return std::accumulate(results.begin(), results.end(), init);
}

int main()
{

	std::vector<int> vec;
	for (int i = 0; i < 10000; ++i)
	{
		vec.emplace_back(i + 1);
	}

	int res = 0;

	int res2 = parallel_accumulate(vec.begin(), vec.end(), res);

	std::cout << "Parellel accumulate: " << res2;

	return 0;
}