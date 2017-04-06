#include <iostream>
#include <string>


template<unsigned n>
struct Arg
{
	template<class X, class ...Xs>
	constexpr auto operator()(X x, Xs... xs)
	{
		return Arg<n - 1>{}(xs...);
	}
};

template<>
struct Arg<0>
{
	template<class X, class ...Xs>
	constexpr auto operator()(X x, Xs...)
	{
		return x;
	}
};

template<unsigned n>
auto arg = Arg<n>{};

int main()
{
	std::cout<<arg<0>("aaaaaaaaaaa", 1, 2, 3, 4, 5);

	return 0;
}
