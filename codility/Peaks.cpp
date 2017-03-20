// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <algorithm>
#include <vector>
int solution(std::vector<int> &A)
{
    std::vector<int> vPeakIndex;

   for(int vIdx=1; vIdx < A.size()-1; ++vIdx)
   {
       if (A.at(vIdx - 1) < A.at(vIdx) && A.at(vIdx + 1) < A.at(vIdx))
           vPeakIndex.emplace_back(vIdx);
   }

   if (vPeakIndex.empty())
       return 0;

   int vMaxSlices = vPeakIndex.size();

   bool vFound = false;

   while(vMaxSlices > 1)
   {
       if(A.size() % vMaxSlices == 0)
       {
           const int kStep = (A.size() / vMaxSlices);

           vFound = true;

           auto vItr = vPeakIndex.begin();

           for(int vIDX = 0; vIDX < A.size(); vIDX += kStep)
           {
               const auto vCurIdx = std::find_if(vItr, vPeakIndex.end(), [&](int aArg)
               {
                   return aArg >= vIDX && aArg <= (vIDX + kStep - 1);
               });

               if(vCurIdx == vPeakIndex.end())
               {
                   vFound = false;
                   break;
               }

               vItr = vCurIdx;
           }
       }

       if (vFound)
           break;

       --vMaxSlices;
   }

   return vMaxSlices;
}