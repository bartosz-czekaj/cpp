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

int factorial(int N)
{
	int res = 1;
	for (int i = N; i > 1; --i)
	{
		res *= i;
	}

	std::cout << "Result is: " << res << std::endl;
	return res;
}

std::deque <std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;

void threadFun()
{
	std::unique_lock<std::mutex> locker(mu);

	cond.wait(locker,[]() {
		return !task_q.empty();
	});

	std::packaged_task<int()> t;
	{
		t = std::move(task_q.front());
		task_q.pop_front();
	}
	t();

}

/* Three ways to get a future
 * - promise::get_future()
 * - packaged_task::get_future()
 * - async() returns a future
*/

int main()
{

	std::thread t1(threadFun);

	std::packaged_task<int()> t_bind(std::bind(factorial, 8));
	std::future<int> fu = t_bind.get_future();

	task_q.emplace_back(std::move(t_bind));
	task_q.emplace_back(std::packaged_task<int()> (std::bind(factorial, 10)));
	cond.notify_one();

	
	t1.join();

	std::cout << "FUTURE: " << fu.get() << std::endl;

	return 0;
}