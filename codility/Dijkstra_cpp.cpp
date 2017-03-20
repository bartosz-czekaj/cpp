#include <vector>
#include <map>
#include <iostream>
#include <set>
using namespace std;

int solution(const vector<int> &A, const vector<int> &B, const vector<int> &C, const vector<int> &D) 
{
    int iRet = -1;
    int iIdx = 0;
    
    std::map<int, std::vector< std::pair<int, int> > > mapVertexCost;
    std::map<int, int> mapVertexPath;
    set<pair<int, int> > setProceeded;
    
    std::vector<std::pair<int, int> > vecTimeToGo(D.size());
    
    for(iIdx = 0; iIdx < A.size(); ++iIdx)
    {
        mapVertexCost[A[iIdx]].push_back(make_pair(B[iIdx], C[iIdx]));
        mapVertexCost[B[iIdx]].push_back(make_pair(A[iIdx], C[iIdx]));
    }
    
    mapVertexPath[0] = 0;
    
    for(iIdx = 1; iIdx < D.size(); ++iIdx)
    {
        vecTimeToGo[iIdx] = make_pair(iIdx, D[iIdx]);
        mapVertexPath[iIdx] = -1;
    }
    
    
    
    
    setProceeded.insert(make_pair(0, 0));
    
    while(setProceeded.empty() == false)
    {
        int iVertex = setProceeded.begin()->second;
        
        setProceeded.erase(setProceeded.begin());
        
        for(std::vector< std::pair<int, int> > ::iterator it = mapVertexCost[iVertex].begin(); 
            it != mapVertexCost[iVertex].end(); 
            ++it)
        {
            int iVertex2 = it->first;
            int iCost = it->second;
            
            if (mapVertexPath[iVertex2] == - 1 || mapVertexPath[iVertex2] > mapVertexPath[iVertex] + iCost)
            {
                if (mapVertexPath[iVertex2] != -1)
                {
                    
                    std::set<pair<int, int> >::iterator itTmp = setProceeded.find(make_pair(mapVertexPath[iVertex2], iVertex2));
                    if(itTmp != setProceeded.end())
                        setProceeded.erase(itTmp);
                }
                mapVertexPath[iVertex2] = mapVertexPath[iVertex] + iCost;
                if(D[iVertex2] > mapVertexPath[iVertex2] && (iRet == -1 || iRet >  mapVertexPath[iVertex2]))
                    iRet = mapVertexPath[iVertex2];
                setProceeded.insert(make_pair(mapVertexPath[iVertex2], iVertex2));
            }
        }
    }
    return iRet;
}