#include <functional>
#include <iostream>
#include <string>
#include <future>
#include <iostream>
#include <mutex>
#include <memory>

template<typename T>
class threadsafe_queue
{
private:

	struct node
	{
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
	};

	std::mutex head_mutex;
	std::unique_ptr<node> head;
	std::mutex tail_mutex;
	node* tail;
	std::condition_variable data_cond;

	node* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}

	std::unique_ptr<node> pop_head()
	{
		std::unique_ptr<node> old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}
	std::unique_lock<std::mutex> wait_for_data()
	{
		std::unique_lock<std::mutex> head_lock(head_mutex);
		data_cond.wait(head_lock, [&] {return head.get() != get_tail(); });
		return std::move(head_lock);
	}
	std::unique_ptr<node> wait_pop_head()
	{
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		return pop_head();
	}
	std::unique_ptr<node> wait_pop_head(T& value)
	{
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		value = std::move(*head->data);
		return pop_head();
	}
public:

	threadsafe_queue()
		:head(new node)
		, tail(head.get())
	{}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_ptr<node> const old_head = wait_pop_head();
		return old_head->data;
	}

	void wait_and_pop(T& value)
	{
		std::unique_ptr<node> const old_head = wait_pop_head(value);
	}

	bool isEmpty()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
			return true;
		return false;
	}

	void push(T new_value)
	{
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));

		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->data = new_data;
			node* const new_tail = p.get();
			tail->next = std::move(p);
			tail = new_tail;
		}

		data_cond.notify_one();
	}
};

void push(threadsafe_queue<int> &tsq, bool isEven)
{
	for (int i = 0; i < 100; ++i)
	{
		if (i % 2 == (isEven ? 0 : 1))
		{
			tsq.push(i);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}

void pop(threadsafe_queue<int> &tsq)
{
	while (!tsq.isEmpty())
	{
		if (auto value = tsq.wait_and_pop())
		{
			std::cout << " " << *value;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(70));
	}
}

int main()
{
	threadsafe_queue<int> tsq;

	auto t1 = std::thread(push, std::ref(tsq), true);
	auto t2 = std::thread(push, std::ref(tsq), false);
	auto t3 = std::thread(pop, std::ref(tsq));

	//std::async(std::launch::async, push, std::ref(tsq), true);
	//std::async(std::launch::async, push, std::ref(tsq), false);
	//std::async(std::launch::async, pop, std::ref(tsq));

	t1.join();
	t2.join();
	t3.join();

	return 0;
}

