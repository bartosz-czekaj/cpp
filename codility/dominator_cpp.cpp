int solution(const std::vector<int> &A) 
{
   int iDominator = -1;
   int iDominatorIdx = -1;
   int iDominatroFound = 0;
   
   if(A.empty() == false)
   {
       std::vector<int> myvec = A;   
       std::sort(myvec.begin(), myvec.end());

       int iJump = myvec.size() / 2;
       int iIdx = 0;

        do{
            if(myvec[iIdx] == myvec[iIdx + iJump])
            {
                iDominator = myvec[iIdx];
                iDominatroFound = 1;
                break;
            }
            ++iIdx;

        }while(iIdx + iJump < myvec.size());

        if(iDominatroFound)
        {
            std::vector<int>::const_iterator it;
            for(it = A.begin(), iIdx = 0; it != A.end(); ++it, ++iIdx)
            {
                if(*it == iDominator)
                {
                    iDominatorIdx = iIdx;
                    break;
                }
            }
        }
   }
   return iDominatorIdx;
 
}
