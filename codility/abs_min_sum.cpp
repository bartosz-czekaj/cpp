#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <iostream> 
using namespace std;


bool myfunction (std::pair<int, int> i,std::pair<int, int> j) 
{ 
    return (i.second < j.second); 
}

bool myfunction2 (std::pair<int, int> i,std::pair<int, int> j) 
{ 
    std::cout<<" i: "<<i.first<<" i "<<i.second<<std::endl;
    std::cout<<" j: "<<i.first<<" j "<<j.second<<std::endl;
    return (i.first < j.first); 
}


vector<int> solution(const vector<int> &A) 
{
    std::vector<pair<int, int> > vecData;
    std::vector<int> vecRet(A.size(), 0);
    std::map<int, std::vector<int> > mapBigger;
    
    int iIdx = 0;
    
    for(std::vector<int>::const_iterator cit = A.begin(); cit != A.end(); ++cit, ++iIdx)
    {
        vecData.push_back(make_pair(iIdx, *cit));
    }
    
    std::sort(vecData.begin(), vecData.end(), myfunction);
    
    iIdx = 0;
    
    for(std::vector<pair<int, int> >::reverse_iterator rit = vecData.rbegin(); rit != vecData.rend(); ++rit, ++iIdx)
    {
         if(rit != vecData.rbegin())
         {
             std::vector<pair<int, int> >::reverse_iterator ritTmp = vecData.rbegin();
             int iTmp = A.size();
             do{
                if(ritTmp->second > rit->second)
                {
                     if(iTmp > std::abs(rit->first - ritTmp->first))
                         iTmp = std::abs(rit->first - ritTmp->first);
                }
                ++ritTmp;
             }while(ritTmp != vecData.rbegin()+iIdx);
             vecRet[rit->first] = iTmp;
         }
    }


    return vecRet;
}

struct myclass 
{
private:
    int m_var;
public:
  myclass(int var) : m_var(var)
  {
      
  }
    
  bool operator() (pair<int, int> i, pair<int, int> j) 
  { 
      std::cout<<"m_var: "<<m_var<<" i: "<<i.first<<" j "<<j.first<<std::endl;
      return i.first < j.first; 
  }
};


struct myclass2
{
public:
  bool operator() (pair<int, int> i, pair<int, int> j) 
  { 
      std::cout<<" i: "<<i.first<<" j "<<j.first<<std::endl;
      return i.first < j.first; 
  }
}myobj;

vector<int> solution2(const vector<int> &A)
{
    std::vector<int> vecRet(A.size(), 0);
    std::vector<pair<int, int> > vecData;
    int iMax = A[0];
    int iIdx = 0;
    
    for(std::vector<int>::const_iterator cit = A.begin(); cit != A.end(); ++cit, ++iIdx)
    {
        vecData.push_back(make_pair(iIdx, *cit));
    }
    
    std::sort(vecData.begin(), vecData.end(), myfunction);
    std::make_heap (vecData.begin(),vecData.end(), myfunction);
    //std::sort_heap (vecData.begin(),vecData.end());
    
    for(std::vector<pair<int, int> >::iterator it = vecData.begin(); it != vecData.end(); ++it)
    {
        std::cout<<"index: "<<it->first<<" value: "<<it->second<<std::endl;
        
    }
    
    
    return vecRet;
}

int main()
{
    std::vector<int> A(9);
    
  A[0] = 4;     A[1]  = 3;    A[2]  = 1;
  A[3] = 4;     A[4]  = -1;   A[5]  = 2; 
  A[6] = 1;     A[7]  = 5;    A[8]  = 7;
    
  solution2(A);
  
    return 0;
}
