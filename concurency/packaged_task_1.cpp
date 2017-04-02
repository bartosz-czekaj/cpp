#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <string>
#include <future>

int factorial(int N)
{
	int res = 1;
	for (int i = N; i > 1; --i)
	{
		res *-i;
	}

	std::cout << "Result is: " << res << std::endl;
	return res;
}

int main()
{
	std::packaged_task<int(int)> t(factorial);
	std::packaged_task<int()> t_bind(std::bind(factorial, 8));

	////*
	//
	//
	//
	/////

	t(8);
	int res = t.get_future().get();
	t_bind();

	return 0;
}