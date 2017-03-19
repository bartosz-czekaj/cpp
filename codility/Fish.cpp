#include <vector>
#include <stack>

int solution(const std::vector<int> &A, const std::vector<int> &B)
{
    std::stack<std::pair<int, bool>> vElements;

    const bool kUP = true;
    const bool kDOWN = false;

    int vIDX = 0;

    for(const auto & vFish : A)
    {
        const bool kCurrentUp = (B[vIDX] == 0);

        if (vElements.empty())
            vElements.push(std::make_pair(vFish, kCurrentUp));
        else
        {
            while(1)
            {
                if(vElements.empty())
                {
                    vElements.push(std::make_pair(vFish, kCurrentUp));
                    break;
                }

                if(vElements.top().second == kCurrentUp)
                {
                    vElements.push(std::make_pair(vFish, kCurrentUp));
                    break;
                }

                if (vElements.top().second == kDOWN &&  kCurrentUp && vElements.top().first > vFish)
                {
                    break;
                }

                if(vElements.top().second == kDOWN &&  kCurrentUp && vElements.top().first < vFish)
                {
                    vElements.pop();
                }
                else if(vElements.top().second == kUP && !kCurrentUp)
                {
                    vElements.push(std::make_pair(vFish, kCurrentUp));
                    break;
                }
            }
        }


        ++vIDX;
    }

    return vElements.size();
}