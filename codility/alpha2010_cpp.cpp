#include <vector>
#include <map>

int solution(vector<int> &A) 
{
  int iRet = -1;
  int iIdx = 0;
  std::map<int, bool> mapExist;
  
  for(std::vector<int>::const_iterator cit = A.begin(); cit != A.end(); ++cit, ++iIdx)
  {
      if(mapExist[*cit] == false)
      {
          mapExist[*cit] = true;
          iRet = iIdx;
      }
  }
  
  
  return iRet;

}