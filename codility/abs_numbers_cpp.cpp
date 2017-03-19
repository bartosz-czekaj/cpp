// you can also use includes, for example:

#include <algorithm>    // std::unique, std::distance
#include <vector>       // std::vector

bool myfunction (int i,int j) 
{ 
    return (abs(i) < abs(j)); 
}

bool myfunction2 (int i,int j) 
{ 
    return (abs(i) == abs(j)); 
}

int solution(const std::vector<int> &A) 
{
    std::vector<int> myvector = A;
    std::vector<int>::iterator it;
    
    std::sort(myvector.begin(), myvector.end(), myfunction);
   
    it =  std::unique (myvector.begin(), myvector.end(), myfunction2);
    myvector.resize( std::distance(myvector.begin(),it) );
    
    
    return myvector.size();
}
