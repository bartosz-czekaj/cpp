#include <thread>
#include <atomic>
#include <iostream>
#include <vector>

std::atomic<int> x(0), y(0), z(0);
std::atomic<bool> go(false);

struct read_values
{
	int x, y, z;
};


void increment(std::atomic<int>& var_to_inc, std::vector<read_values>& values)
{
	while (!go)
	{
		std::this_thread::yield();//tells the implementation to reschedule the execution of threads, that should be used in a case where you are in a busy waiting state
	}

	int i = 0;
	for (auto &v : values)
	{
		v.x = x.load(std::memory_order_relaxed);
		v.y = y.load(std::memory_order_relaxed);
		v.z = z.load(std::memory_order_relaxed);

		var_to_inc.store(++i, std::memory_order_relaxed);

		std::this_thread::yield();//tells the implementation to reschedule the execution of threads, that should be used in a case where you are in a busy waiting state
	}
}
void read_vals(std::vector<read_values>& values)
{
	while (!go)
	{
		std::this_thread::yield();//tells the implementation to reschedule the execution of threads, that should be used in a case where you are in a busy waiting state
	}

	for (auto &v : values)
	{
		v.x = x.load(std::memory_order_relaxed);
		v.y = y.load(std::memory_order_relaxed);
		v.z = z.load(std::memory_order_relaxed);

		std::this_thread::yield();//tells the implementation to reschedule the execution of threads, that should be used in a case where you are in a busy waiting state
	}
}
void print(std::vector<read_values>& vec)
{
	int i = 0;
	for (const auto &v : vec)
	{
		if (i++) std::cout << ",";

		std::cout << "(" << v.x << "," << v.y << "," << v.z << ")";
	}
	std::cout << std::endl;
}
int main()
{


	unsigned const loop_count = 10;


	std::vector<read_values> values1(loop_count);
	std::vector<read_values> values2(loop_count);
	std::vector<read_values> values3(loop_count);
	std::vector<read_values> values4(loop_count);
	std::vector<read_values> values5(loop_count);


	std::thread t1(increment, std::ref(x), std::ref(values1));
	std::thread t2(increment, std::ref(y), std::ref(values2));
	std::thread t3(increment, std::ref(z), std::ref(values3));
	std::thread t4(read_vals, std::ref(values4));
	std::thread t5(read_vals, std::ref(values5));
	
	go = true;
	
	t5.join();
	t4.join();
	t3.join();
	t2.join();
	t1.join();


	print(values1);
	print(values2);
	print(values3);
	print(values4);
	print(values5);


	return 0;
}