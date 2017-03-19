#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <deque>

std::mutex mu;
std::deque<int> q;

std::condition_variable cond;

void function_1()
{
	int count = 10;
	while (count > 0)
	{
		std::unique_lock<std::mutex> locker(mu);
		q.emplace_front(count);

		locker.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		cond.notify_one();//wakes up only one thread
		//cond.notify_all();//wakes up all threads
		--count;
	}
}

void function_2()
{
	int data = 0;
	while (data != 1)
	{
		std::unique_lock<std::mutex> locker(mu);

		cond.wait(locker, []()
		{
			return q.empty() == false;
		});//prevents spurious wake

		data = q.back();
		q.pop_back();
		locker.unlock();

		std::cout << " t2 got value from t1: " << data << std::endl;
	}
}


int main()
{
	std::thread  t1(function_1);
	std::thread  t2(function_2);

	t1.join();
	t2.join();

	return 0;
}