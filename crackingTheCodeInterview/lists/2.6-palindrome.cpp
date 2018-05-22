#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stack>
#include "list.h"


struct testData
{
    List data;
    bool expected;
};

auto addToStack(std::stack<int> &stck)
{
    return [&]( auto&  x){
        stck.push(x);
    };
}

auto removeFromStack(std::stack<int> &stck)
{
    return [&]( auto&  x){
        if(stck.top() == x)
        {
            stck.pop();
        }
    };
}

bool checkPalindrome(Node *node)
{
    std::stack<int> stck;

    auto iteration = [&](auto &&action)
    {
        auto current = node;
        while(current != nullptr)
        {
            action(current->data);
            current = current->next;
        }
    };

    iteration(addToStack(stck));
    iteration(removeFromStack(stck));

    return stck.empty();
}

void test(const std::vector<testData> &td)
{
    auto setOfData = 0;
    for(auto& d : td)
    {
        if(checkPalindrome(d.data.head) != d.expected)
        {
            std::cout<<"Error "<<(setOfData+1)<<'\n';
        }
        ++setOfData;
    }
}

int main()
{
    List l;
    l.insert(1);
    l.insertAtEnd(2);
    l.insertAtEnd(2);
    l.insertAtEnd(1);

    List l1;
    l1.insert(1);
    l1.insertAtEnd(2);
    l1.insertAtEnd(3);
    l1.insertAtEnd(4);
    l1.insertAtEnd(3);
    l1.insertAtEnd(2);
    l1.insertAtEnd(1);

    List l2;
    l2.insert(1);
    l2.insertAtEnd(2);
    l2.insertAtEnd(1);
    l2.insertAtEnd(2);

    List l3;
    l3.insert(11);
    l3.insertAtEnd(222);
    l3.insertAtEnd(31);
    l3.insertAtEnd(14);
    l3.insertAtEnd(25);
    l3.insertAtEnd(36);
    l3.insertAtEnd(47);

    List l4;
    l4.insert(11);
    l4.insertAtEnd(11);
    l4.insertAtEnd(11);
    l4.insertAtEnd(11);
    l4.insertAtEnd(11);
    l4.insertAtEnd(11);
    l4.insertAtEnd(11);


    std::vector<testData> td;

    td.emplace_back(testData{l,true});
    td.emplace_back(testData{l1,true});
    td.emplace_back(testData{l2,false});
    td.emplace_back(testData{l3,false});
    td.emplace_back(testData{l4,true});
   
    test(td);

    return 0;
}