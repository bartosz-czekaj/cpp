#include <iostream>
#include <string>
#include <chrono>
#include <functional>

struct Base
{
	virtual void DoIt() = 0;

protected:
	template<typename T>
	static void ShowSizeOfType()
	{
		std::cout << sizeof(T) << std::endl;
	}

	template<class TI>
	void Action()
	{
		ShowSizeOfType<TI>();
	}

	template<typename TI, typename U, typename ...T>
	void Action()
	{
		ShowSizeOfType<TI>();
		Action<U, T...>();
	}

	
	void Action() {}

};

template<typename ...T>
struct  Derived : Base
{
	void DoIt()
	{
		Action<T...>();
	}

protected:

};

int main()
{
	Derived<int, float, double, long long, long, char, unsigned int,short, unsigned > d;

	d.DoIt();

	return 0;
}