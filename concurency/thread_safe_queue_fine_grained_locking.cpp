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
	
	node* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}
	
	std::unique_ptr<node> pop_head()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return nullptr;
		}
		std::unique_ptr<node> old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}

public:
	threadsafe_queue() 
		:head(new node)
		,tail(head.get())
	{}

	bool isEmpty()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
			return true;
		return false;
	}
	
	std::unique_ptr<node> pop_head()//This is a broken implementation
	{
		//Get old tail value outside lock on head_mutex
		node* const old_tail=get_tail();
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if(head.get()==old_tail)
		{
			return nullptr;
		}
		std::unique_ptr<node> old_head=std::move(head);
		head=std::move(old_head->next);
		return old_head;
	}

	threadsafe_queue(const threadsafe_queue& other) = delete;
	threadsafe_queue& operator=(const threadsafe_queue& other) = delete;

	std::shared_ptr<T> try_pop()
	{
		std::unique_ptr<node> old_head = pop_head();
		return old_head ? old_head->data : std::shared_ptr<T>();
	}

	void push(T new_value)
	{
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<node> p(new node);
		node* const new_tail = p.get();
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		tail->data = new_data;
		tail->next = std::move(p);
		tail = new_tail;
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
		if (auto value = tsq.try_pop())
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