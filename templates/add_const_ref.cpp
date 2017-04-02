#include <iostream>
#include <string>

template <typename T, bool convert>
struct add_const_ref;

template<typename T>
struct add_const_ref<typename T, true>
{
	using type = T;
};

template<typename T>
struct add_const_ref<typename T, false>
{
	using type = const T&;
};

template<typename T> 
struct add_const_ref_imp : add_const_ref<T, std::is_reference<T>::value>
{};


int main()
{
	int x = 0;
	float f = 0;

	add_const_ref_imp<int>::type x1 = x;
	add_const_ref_imp<float &>::type f1 = f;

	x = 2;
	f = 2.2;
	
	std::cout << "x: " << x1 << std::endl;
	std::cout << "f: " << f1 << std::endl;

	return 0;
}