#include <iostream>

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
	std::cout << std::is_same<add_const_ref_imp<int>::type, const int &>::value << std::endl;
	std::cout << std::is_same<add_const_ref_imp<float &>::type, float &>::value << std::endl;

	return 0;
}