#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;


int main() 
{
    std::map<int, int> map {{1,1}, {2,2}, {3,4}};
    
    int games = 0;
    std::cin >> games;
    
    while(games-- > 0)
    {
        int n = 0;
        std::cin >> n;
    
        for(int nb = 4; nb <= n; ++nb)
        {
            if (map.find(nb) == map.end())
            {
                map[nb] = map[nb-1]+map[nb-2]+map[nb-3];
            }
        }
        
        std::cout<<map[n]<<std::endl;
    }
    
    return 0;
}