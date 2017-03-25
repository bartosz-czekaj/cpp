#include <iostream>
#include <initializer_list>

template<typename T>
void print_imp(const T& t)
{
	std::cout << t << "\n";
}

template<typename ...T>
void print(const T& ...t)
{
	(void)std::initializer_list<int>{(print_imp(t),0)...};
	(void)std::initializer_list<int>{(std::cout<<"second version: "<<t<<"\n", 0)...};
}

int main()
{
	print(1, "ala ma kota", 0.3, 22.22f, 99u);

	return 0;
}