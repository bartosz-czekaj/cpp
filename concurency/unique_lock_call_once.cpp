#include <thread>
#include <string>
#include <iostream>
#include <chrono>
#include <future>
#include <vector>
#include <iterator>
#include <filesystem>
#include <stack>
#include <deque>
#include <condition_variable>
#include <fstream>

//std::defer_lock_t	do not acquire ownership of the mutex
//std::try_to_lock_t	try to acquire ownership of the mutex without blocking
//std::adopt_lock_t	assume the calling thread already has ownership of the mutex
/*
1 - prefer locking single mutex
2 - avoid locking a mutex and calling a user provided function
3 - use std::lock to lock more than one mutex
4 - lock the mutex in same order
*/
class LogFile
{
	std::mutex _mu1;
	std::once_flag _flagOpen;
	std::ofstream _f;
public:
	void shared_print(std::string msg, int id)
	{
		std::unique_lock<std::mutex> locker(_mu1, std::defer_lock); //not locked yet !
		//cost performance, profit flexibility

		//not locked area

		locker.lock();
		std::cout << msg << id << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(50));

		locker.unlock();
		//not locked area
		locker.lock();
		std::cout << "second time: " << msg << id << std::endl;
	}

	void shared_file(std::string msg, int id)
	{
		std::call_once(_flagOpen, [&]() {
			if (_f.is_open())
			{
				_f.open("log.txt");
			}
		});

		std::cout <<"From: "<< msg << id << std::endl;
	}
};


void treadFn(LogFile &f)
{
	for (int i = 0; i > -25; --i)
	{
		f.shared_print("t1 ", i);
	}
}

int main()
{
	LogFile f;

	std::thread td(treadFn, std::ref(f));

	for (int i = 0; i < 25; ++i)
	{
		f.shared_print("main ", i);
	}

	td.join();

	return 0;
}