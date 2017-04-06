#include <iostream>
#include <string>
#include <list>
#include <shared_mutex>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>

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

	template<typename TI>
	void push_front_impl(TI&& t, std::true_type)
	{
		push_front(static_cast<T>(t));
	}

	template<typename TI>
	void push_front_impl(TI&& t, std::false_type)
	{
		
	}

public:
	threadsafe_list(){}

	~threadsafe_list()
	{
		remove_if([](node const&) {return true; });
	}

	threadsafe_list(threadsafe_list const& other) = delete;
	threadsafe_list& operator=(threadsafe_list const& other) = delete;
	
	void push_front(const T &value)
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

	template<typename ...Type>
	void push_front_multi_variadic(const Type& ...t)
	{
		(void)std::initializer_list<int>{(push_front_impl(t, std::integral_constant<bool, (std::is_same<Type, T>::value || std::is_convertible<Type, T>::value)>()), 0)...};
	}

	template<typename ...Type>
	std::thread push_front_multi_variadic_th(const Type& ...t)
	{
		return std::thread([&]()
		{
			try
			{
				(void)std::initializer_list<int>{(push_front_impl(t, std::integral_constant<bool, (std::is_same<Type, T>::value || std::is_convertible<Type, T>::value)>()), 0)...};
			}
			catch (...)
			{

			}

			const auto &fun = [](const auto &arg)
			{
				std::thread::id this_id = std::this_thread::get_id();
				std::stringstream ss;;
				ss.str("");
				ss << this_id << ": " << arg << std::endl;

				std::cout << ss.str();
			};

			for_each(fun);

		});
	}
};

template<typename T>
void run(threadsafe_list<T> &data)
{
	data.push_front_multi_variadic("1s", "2s", "3s", 11, 22, 33, 44, 55, "4s", "5s");
	const auto &fun = [](const auto &arg)
	{
		std::thread::id this_id = std::this_thread::get_id();
		std::stringstream ss;;
		ss.str("");
		ss << this_id << ": " << arg << std::endl;
		
		std::cout <<ss.str();
	};

	data.for_each(fun);
}

int main()
{
	threadsafe_list<std::string> tsl_str;
	threadsafe_list<int> tsl_int;

	std::thread t1 = tsl_str.push_front_multi_variadic_th("aaaaa", "bbbbbbbbb", "cccc", 1, 2, 3, "ddd", "fff");
	std::thread t2 = tsl_int.push_front_multi_variadic_th("aaaaa", "bbbbbbbbb", "cccc", 1, 2, 3, "ddd", "fff");

	std::thread t3(run<std::string>, std::ref(tsl_str));
	std::thread t4(run<int>, std::ref(tsl_int));

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return 0;
}
