#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <string>
#include <future>
#include <deque>
#include <shared_mutex>

class test
{
private:
	std::recursive_mutex _mu; //to avoid deadlock; bad design
	int _member;
public:
	test() : _member(0){}

	void add(int value)
	{
		std::lock_guard<std::recursive_mutex> lock(_mu);
		_member += value;
	}

	void increment()
	{
		std::lock_guard<std::recursive_mutex> lock(_mu);
		add(1);
	}

};

int main()
{
	test t;
	std::thread t1(&test::add, &t, 5);
	std::thread t2(&test::increment, &t);

	t1.join();
	t2.join();

	return 0;
}