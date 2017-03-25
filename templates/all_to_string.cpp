#include <iostream>
#include <initializer_list>
#include <string>
#include <vector>
#include <sstream>

template<typename ...T>
std::vector<std::string> AlltoString(const T& ...t)
{
	std::stringstream ss;
	std::vector<std::string> retVal;

	(void)std::initializer_list<int>{(ss.str(""), ss<<t, retVal.emplace_back(ss.str()), 0)...};

	return retVal;
}

int main()
{
	auto vStrings= AlltoString(1,0x67AC, 0xFFFFF, 0.3, 22.22f, 99u);

	for (const auto &vStr : vStrings)
	{
		std::cout << vStr << std::endl;
	}

	return 0;
}