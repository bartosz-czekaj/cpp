#include <vector>
#include <algorithm>

int solution(std::vector<int> &A)
{
    std::sort(A.begin(), A.end());

    std::vector<int>::iterator it;
    it = std::unique(A.begin(), A.end());

    A.resize(std::distance(A.begin(), it));

}