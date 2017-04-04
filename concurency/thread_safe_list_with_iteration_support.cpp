#include <iostream>
#include <string>
#include <list>
#include <shared_mutex>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>

class Thread
{
private:

	std::exception_ptr exceptionPtr;
	std::thread thread;

public:

	using Id = std::thread::id;

	using NativeHandleType = std::thread::native_handle_type;

	Thread() noexcept = default;
	Thread(Thread &&t) noexcept :
	exceptionPtr(std::move(t.exceptionPtr)),
		thread(std::move(t.thread))
	{
	}

	Thread &operator =(Thread &&t) noexcept
	{
		exceptionPtr = std::move(t.exceptionPtr);
		thread = std::move(t.thread);
		return *this;
	}

	template<typename Callable, typename... Args>
	Thread(Callable &&f, Args &&... args) :
		exceptionPtr(nullptr),
		thread([&](typename std::decay<Callable>::type &&f, typename std::decay<Args>::type &&... args)
	{
		try
		{
			std::bind(f, args...)();
		}
		catch (...)
		{
			exceptionPtr = std::current_exception();
		}

	}, std::forward<Callable>(f), std::forward<Args>(args)...)
	{
	}

	bool joinable() const noexcept
	{
		return thread.joinable();
	}

	void join()
	{
		thread.join();

		if (exceptionPtr != nullptr)
		{
			std::rethrow_exception(exceptionPtr);
		}
	}

	void detach()
	{
		thread.detach();
	}

	Id getId() const noexcept
	{
		return thread.get_id();
	}

	NativeHandleType nativeHandle()
	{
		return thread.native_handle();
	}

	static uint32_t hardwareConcurrency() noexcept
	{
		return std::thread::hardware_concurrency();
	}


};

template<typename T>
class threadsafe_list
{
	struct node
	{
		std::mutex m;
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
		node() 
			: next()
		{}

		node(T const& value) 
			: data(std::make_shared<T>(value))
		{}
	};
	node head;
public:
	threadsafe_list(){}

	~threadsafe_list()
	{
		remove_if([](node const&) {return true; });
	}

	threadsafe_list(threadsafe_list const& other) = delete;
	threadsafe_list& operator=(threadsafe_list const& other) = delete;
	
	void push_front(T const& value)
	{
		std::unique_ptr<node> new_node(new node(value));
		std::lock_guard<std::mutex> lk(head.m);
		new_node->next = std::move(head.next);
		head.next = std::move(new_node);
	}

	template<typename Function>
	void for_each(Function f)
	{
		node* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while (node* const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			lk.unlock();
			f(*next->data);
			current = next;
			lk = std::move(next_lk);
		}
	}

	template<typename Predicate>
	std::shared_ptr<T> find_first_if(Predicate p)
	{
		node* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while (node* const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			lk.unlock();
			if (p(*next->data))
			{
				return next->data;
			}
			current = next;
			lk = std::move(next_lk);
		}
		return std::shared_ptr<T>();
	}

	template<typename Predicate>
	void remove_if(Predicate p)
	{
		node* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while (node* const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			if (p(*next->data))
			{
				std::unique_ptr<node> old_next = std::move(current->next);
				current->next = std::move(next->next);
				next_lk.unlock();
			}
			else
			{
				lk.unlock();
				current = next;
				lk = std::move(next_lk);
			}
		}
	}

	std::thread push_front_multi(std::vector<T> &list)
	{
		return std::thread([&] ()
		{ 
			for (const auto &elem : list)
			{
				push_front(elem);
			}
		});
	}

	template<typename ...T>
	std::thread push_front_multi_variadic(const T& ...t)
	{
		const auto &variadic_generic_lambda = [&](auto&&... param) 
		{
			std::stringstream ss;

			(void)std::initializer_list<int>{(ss.str(""), ss << t, push_front(ss.str()), 0)...};

		};
		
			;		
		//Thread th(variadic_generic_lambda, t...);
		//th.
		//
		return std::thread([&]()
		{
			try
			{
				std::stringstream ss;

				(void)std::initializer_list<int>{(ss.str(""), ss << t, push_front(ss.str()), 0)...};
				//std::bind(variadic_generic_lambda, t...)();
			}
			catch (...)
			{

			}
		});
	}
};






int main()
{
	
	threadsafe_list<std::string> tsl;

	std::vector<std::string> vec{"a", "b", "c", "d", "e"};

	//std::thread t1 = tsl.push_front_multi(std::ref(vec));
	//std::thread t2 = tsl.push_front_multi(std::ref(vec));
	//std::thread t3 = tsl.push_front_multi(std::ref(vec));

	std::thread t4 = tsl.push_front_multi_variadic("aaaaa", "bbbbbbbbb", "cccc", 1, 2, 3);

	//std::thread t1(&threadsafe_list<std::string>::push_front, tsl, "asa");

	//t1.join();
	//t2.join();
	//t3.join();
	t4.join();

	const auto &fun = [](const std::string &arg)
	{
		std::cout << arg << std::endl;
	};

	tsl.for_each(fun);

	return 0;
}
