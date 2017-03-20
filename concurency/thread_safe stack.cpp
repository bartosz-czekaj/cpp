#include <condition_variable>
#include <mutex>
#include <stack>
#include <chrono>
#include <exception>
#include <string>
#include <iostream>

struct empty_stack : std::exception
{
	const char* what() const throw();

};

const char* empty_stack::what() const
{
	return "ERROR";
}

template<typename T>
class threadsafe_stack
{
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}


	threadsafe_stack& operator=(const threadsafe_stack&) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));
	}
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();
		std::shared_ptr<T> const res(
			std::make_shared<T>(std::move(data.top())));
		data.pop();
		return res;
	}
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();
		value = std::move(data.top());
		data.pop();
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};



void fun_push(threadsafe_stack<int> &stack)
{
	int i = 0;
	while (i < 10)
	{
		try
		{
			stack.push(i*2+2);
		}
		catch (const empty_stack &ex)
		{
			continue;
		}

		std::string vS = "fun_push: ";
		vS.append(std::to_string(i * 2 + 2)).append("\n");

		std::cout << vS;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		++i;
	}
}

void fun_pop(threadsafe_stack<int> &stack)
{
	int i = 0;
	while(i < 10)
	{
		int popValue = 0;
		try
		{
			stack.pop(popValue);
		}
		catch (const empty_stack &ex)
		{
			continue;
		}

		std::string vS = "fun_pop: ";
		vS.append(std::to_string(popValue)).append("\n");

		std::cout << vS;

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		++i;
	}
}

int main()
{

	threadsafe_stack<int> ts_s;

	std::thread th1(fun_push, std::ref(ts_s));
	std::thread th2(fun_pop, std::ref(ts_s));


	th1.join();
	th2.join();

	return 0;
}