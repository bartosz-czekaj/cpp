#include <functional>
#include <iostream>
#include <string>

void f(int i)
{
	std::cout << "f " << i << std::endl;
};

auto output_values =
[](auto& os, const auto&... args)
{
	return (void)std::initializer_list<int>{ (os <<"test: "<< args <<"\n ", 0)...};
};

struct S
{
	int v = 6;
	void g(int i)
	{
		std::cout << "S:d " << i << std::endl;
	};
};

int main()
{
	S s;
	S* p = &s;

	std::invoke(f, 3);           // f(3)
	std::invoke(&S::g, s, 4);    // s.g(4)
	std::invoke(&S::g, p, 5);    // p->g(5)
	std::invoke(&S::v, s);       // s.v

	std::invoke(output_values, std::cout, 5, 3.5, std::string("Hello"));


	return 0;
}