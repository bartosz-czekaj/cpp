2147483647
1998136186
999336263

Possible
Possible
Possible
Impossible
Possible
Impossible
Possible
Possible
Possible
Possible

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;


int main() {
	/* Enter your code here. Read input from STDIN. Prlong long output to STDOUT */
	long long all = 0;
	std::cin >> all;
	for (long long i = 0; i < all; ++i)
	{
		long long ballTypes = 0;
		std::cin >> ballTypes;

		std::vector<long long> rowsSum(ballTypes);
		std::vector<long long> colsSum(ballTypes);

        for (long long row = 0; row < ballTypes; ++row)
		{
			long long rowSum = 0;
			for (long long col = 0; col < ballTypes; ++col)
			{
				long long colVal = 0;
				std::cin >> colVal;

				rowSum += colVal;
                colsSum[col] += colVal;
            }
			rowsSum[row] = rowSum;
		}
        std::sort(rowsSum.begin(), rowsSum.end());
        std::sort(colsSum.begin(), colsSum.end());

		if (rowsSum == colsSum)
			std::cout << "Possible\n";
		else
			std::cout << "Impossible\n";
	}

	return 0;
}

