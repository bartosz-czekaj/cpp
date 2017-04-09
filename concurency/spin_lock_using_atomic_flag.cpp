#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include <utility>

class spinlock_mutex
{
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
	spinlock_mutex()
	{
		
	}
	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
	}
	void unlock()
	{
		flag.clear(std::memory_order_release);
	}
};


class spinlock_mutex_guard
{
private:
	spinlock_mutex &_mutex;
public:
	spinlock_mutex_guard(spinlock_mutex &mutex)
		: _mutex(mutex)
	{
		_mutex.lock();
	}

	~spinlock_mutex_guard()
	{
		_mutex.unlock();
	}
};



struct Result
{
	std::vector<int> data;

	Result(Result && res)
		: data(std::move(res.data))
	{}

	Result()
	{}

	Result& operator=(Result&& other)
	{
		data = std::move(other.data);

		return *this;
	}

	Result& operator=(const Result& other)
	{
		if (this != &other)
		{
			data = other.data;
		}
		return *this;
	}
};

class MonitorResult
{
public:
	void PutNumber(int && file)
	{
		spinlock_mutex_guard lock(_mutex);
		_result.data.emplace_back(file);
	}

	bool IsDirEmpty()
	{
		spinlock_mutex_guard lock(_mutex);
		return _result.data.empty();
	}


	void Clear()
	{
		spinlock_mutex_guard lock(_mutex);
		_result.data.clear();
	}

	std::vector<int> &GetNumbers()
	{
		return _result.data;
	}


private:
	spinlock_mutex _mutex;
	Result _result;
};

void AddItems(MonitorResult &mr, bool odd)
{
	for (int i = 0; i < 100; ++i)
	{
		if (i % 2 == (odd ? 1 : 0))
		{
			mr.PutNumber(std::forward<int>(i+1));
			std::this_thread::sleep_for(std::chrono::milliseconds(odd ? 150 : 100));
		}
	}
}

void AddItems2(MonitorResult &mr, bool odd)
{
	for (int i = 1000; i < 1500; ++i)
	{
		if (i % 2 == (odd ? 1 : 0))
		{
			mr.PutNumber(std::forward<int>(i + 1));
			std::this_thread::sleep_for(std::chrono::milliseconds(odd ? 50 : 0));
		}
	}
}

void Clear(MonitorResult &mr)
{
	mr.Clear();
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}


int main()
{
	MonitorResult result;
	std::thread t1(AddItems, std::ref(result), true);
	std::thread t2(AddItems, std::ref(result), false);
	std::thread t3(AddItems2, std::ref(result), true);
	std::thread t4(AddItems2, std::ref(result), false);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout << "ALL: " << result.GetNumbers().size() << std::endl;

	for (const auto nb : result.GetNumbers())
	{
		std::cout << nb << "; ";
	}

	return 0;
}