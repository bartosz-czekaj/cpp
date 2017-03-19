// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <vector>
#include <algorithm>
#include <map>

int solution(std::vector<int> &A)
{
    std::map<int, int> vElems;
    
    for(const auto &vElem : A)
    {
        vElems[vElem]++;
    }
    
    int vDominatorCandidate = 0;
    int vDominatorCandidateCount = 0;
     
    for(const auto &vPair : vElems)
    {
        if(vPair.second > vDominatorCandidateCount)
        {
            vDominatorCandidateCount = vPair.second;
            vDominatorCandidate = vPair.first;
        }
    }
    
    int vIDX = -1;
    
    if(vDominatorCandidateCount)
    {
        const double kHalf = 2.0;
        if(((double)A.size() / (double)vDominatorCandidateCount) < kHalf)
        {
            vIDX = std::find(A.begin(), A.end(), vDominatorCandidate) - A.begin();
        }
    }
    
    return vIDX;
}