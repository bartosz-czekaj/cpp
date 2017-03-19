// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
#include <vector>
#include <map>
#include <algorithm>

int solution(std::vector<int> &A)
{
    std::map<int, int> vElemCount;
    
    for(const auto &vElem : A)
    {
        vElemCount[vElem]++;
    }
    
    int vDominator = 0;
    int vDominatorCount = 0;
    
    for(const auto &vPair : vElemCount)
    {
        if(vPair.second > vDominatorCount)
        {
            vDominatorCount = vPair.second;
            vDominator = vPair.first;
        }
    }
    
    const auto &kMoreThanHalf = [](const int &aSetSize, const int &aMaxNb)
    {
        if(aMaxNb)
        {
            const double kHalf = 2.0;
            
            return(((double)aSetSize / (double)aMaxNb) < kHalf);
        }
        
        return false;
    };
    
    int vRet = 0;
    int vUsedMax = 0;
    int vIdx = 0;
    
    for(const auto &vElem : A)
    {
        ++vIdx;
        
        int vLeft = A.size() - vIdx;
        
        if(vElem == vDominator)
        {
            ++vUsedMax;
            --vDominatorCount;
        }
        
        if(kMoreThanHalf(vIdx, vUsedMax) && kMoreThanHalf(A.size() - vIdx, vDominatorCount))
        {
            ++vRet;
        }
    }
    
    return vRet;
}