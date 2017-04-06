#include <iostream>
#include <string>
#include <list>
#include <shared_mutex>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>

template<class L, class R>
std::string foldFB(L l, R r)
{
	std::stringstream ss;
	ss.str("");
	ss << "The Same L: " << l << " r:" << r << std::endl;

	return ss.str();
}

template<class L, class R>
std::string foldBB(L l, R r)
{
	std::stringstream ss;
	ss.str("");
	ss << "Convertible L: " << l << " r:" << r << std::endl;

	return ss.str();
}

std::string foldFF()
{
	return "Wrong Types!\n";
}

template<class L, class R>
auto fold(L l, R r)
{
	if constexpr(std::is_convertible<L, R>::value)
	{
		if constexpr(std::is_same<L, R>::value)
		{
			return foldFB(l, r);
		}
		else
		{
			return foldBB(l, r);
		}
	}
	else
	{
		return foldFF();
	}

};

int main()
{
	std::cout << fold<std::string, int>("ala ma kota", 1);
	std::cout << fold(2u, 1);
	std::cout << fold(222, 111);
	std::cout << fold("ala", std::string("ma kota"));

	return 0;
}