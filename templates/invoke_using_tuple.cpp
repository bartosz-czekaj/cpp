#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <iostream>
#include <tuple>
#include <functional>

int add(int first, int second)
{
	return first + second;
}

template<typename T>
void print(T first, T second)
{
	std::cout << first << " " << second<<"\n";
}

template <class TCallable, typename TTuple, size_t... index>
constexpr decltype(auto) test_impl(TCallable &&callable, TTuple &&tuple, std::index_sequence<index...>)
{
	return std::invoke(std::forward<TCallable>(callable), std::get<index>(std::forward<TTuple>(tuple))...);
}

template <class TCallable, typename TTuple>
constexpr decltype(auto) test(TCallable &callable, TTuple &tuple)
{
	return test_impl(std::forward<TCallable>(callable), std::forward<TTuple>(tuple), std::make_index_sequence<std::tuple_size_v<TTuple>> {});
}

int main()
{
	auto tuple =std::make_tuple(1, 2) ;

	std::cout << test(add, tuple)<<std::endl;
	test([](auto&&... params) { return print(std::forward<decltype(params)>(params)...); }, tuple);

	return 0;
}