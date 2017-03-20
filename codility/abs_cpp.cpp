#include <vector>
#include <map>
#include <algorithm> 
using namespace std;


bool myfunction (std::pair<int, int> i,std::pair<int, int> j) 
{ 
    return (i.second < j.second); 
}

bool myfunction2 (std::pair<int, int> i,std::pair<int, int> j) 
{ 
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
             do{
                if(ritTmp->second > rit->second)
                        mapBigger[rit->first].push_back(std::abs(rit->first - ritTmp->first));
                 ++ritTmp;
             }while(ritTmp != vecData.rbegin()+iIdx);
         }
    }
    
   
    for(std::map<int, std::vector<int> >::iterator iit = mapBigger.begin(); iit != mapBigger.end(); ++iit)
    {
        vecRet[iit->first] = *std::min_element(iit->second.begin(), iit->second.end());
       
    }
    return vecRet;
}