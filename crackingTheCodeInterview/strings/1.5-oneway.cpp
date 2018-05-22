#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>
#include <cstdlib>

struct testData
{
    std::string data;
    std::string expected;
    bool result;
};

bool oneway(std::string_view data, std::string_view expected)
{
   if(std::abs((int)data.size() - (int)expected.size()) > 1)
   {
       return false;
   }

   std::string_view shorter = data.size() < expected.size() ? data : expected;
   std::string_view longer = data.size() < expected.size() ? expected : data;

   auto shorterS = shorter.size();  
   auto longerS = longer.size();

   auto idxS = 0;
   auto idxL = 0;

   auto changes = 0;

   while(idxS < shorter.size() && idxL < longer.size())
   {
       if(shorter[idxS] != longer[idxL])
       {
           if(changes >= 1)
           {
               return false;
           }
           ++changes;

           if(shorterS == longerS)
           {
              ++idxS;
              ++idxL; 
           }
           else
           {
              ++idxL; 
           }
       }
       else
       { 
        ++idxS;
        ++idxL;
       }
   }
    
   return true;
}

void test(std::vector<testData> &&td)
{
    for(auto& d : td)
    {
        if(oneway(d.data, d.expected) != d.result)       
        {
            std::cout<<"ERROR: "<<d.expected<<' '<<d.data<<'\n';
        }
    }
}


int main()
{
     test(std::vector<testData>{
        {"pale","plepaa", false}, 
        {"pale","ple", true},
        {"pales","pale", true},
        {"pale","bale", true},
        {"pale","bake", false},
        {"kotek","korek", true},
        {"kotek","kot", false},
        {"kotek","koek", true}
    });

    return 0;
}