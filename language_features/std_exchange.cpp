#include <iostream>
#include <utility>
#include <vector>

int main()
{

	std::vector<int> data{1, 2, 3, 4, 5, 7, 8, 9, 10};

	for (auto &v : data)
	{
		std::cout<<std::exchange(v, (v * 2))<<std::endl;
	}
	std::cout << "**********************************\n";
	for (auto &v : data)
	{
		std::cout << v << std::endl;
	}


	int x = 2;
	int y = 3;
	std::cout << "**********************************\n";
	std::cout << "X: " << x << " y: " << y << std::endl;
	
	x = std::exchange(y, x);

	std::cout << "**********************************\n";
	std::cout << "X: " << x << " y: " << y << std::endl;

	return 0;
}