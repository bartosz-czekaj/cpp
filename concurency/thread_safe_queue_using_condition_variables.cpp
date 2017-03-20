#include <condition_variable>
#include <mutex>
#include <queue>
#include <chrono>
#include <exception>
#include <string>
#include <iostream>

template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue()
	{}

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(new_value));
		data_cond.notify_one();
	}
	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		value = std::move(data_queue.front());
		data_queue.pop();
	}
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		
		std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
	
		data_queue.pop();
		return res;
	}
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(
			std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};



void fun_push(threadsafe_queue<int> &queue)
{
	int i = 0;
	while (i < 10)
	{
		queue.push(i * 2 + 2);
		
		std::string vS = "fun_push: ";
		vS.append(std::to_string(i * 2 + 2)).append("\n");

		std::cout << vS;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		++i;
	}
}

void fun_wait_and_pop(threadsafe_queue<int> &queue)
{
	int i = 0;
	while (i < 10)
	{
		int popVal = 0;
		queue.wait_and_pop(popVal);

		std::string vS = "fun_wait_and_pop: ";
		vS.append(std::to_string(popVal)).append("\n");

		std::cout << vS;
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		++i;
	}
}

int main()
{

	threadsafe_queue<int> ts_q;

	std::thread th1(fun_push, std::ref(ts_q));
	std::thread th2(fun_wait_and_pop, std::ref(ts_q));


	th1.join();
	th2.join();

	return 0;
}