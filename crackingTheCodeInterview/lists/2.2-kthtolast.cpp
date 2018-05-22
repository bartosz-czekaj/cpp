#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include "list.h"


Node* getNthToLastRec(Node *head, int kth, int &i)
{
    if(head == nullptr)
    {
        return nullptr;
    }

    Node *nd = getNthToLastRec(head->next, kth, i);
    ++i;

    if(i == kth)
    {
        return head;
    }
    return nd;
}

Node* getNthToLast(const List &list, int kth)
{
    auto ptr1 = list.head;
    auto ptr2 = list.head;

    for(auto i = 0; i < kth; ++i )
    {
        if(ptr1 == nullptr)
            return nullptr;
        ptr1 = ptr1->next;    
    }

    while(ptr1 != nullptr)
    {
        ptr1 = ptr1->next;  
        ptr2 = ptr2->next;  
    }

    return ptr2;
}

struct testData
{
    List data;
    int nthToEnd;
    int expected;
};

void testRec(const std::vector<testData> &td)
{
    for(auto& d : td)
    {
        int i = 0;
        auto ptr = getNthToLastRec(d.data.head, d.nthToEnd, i);
        if(ptr == nullptr)
        {
            std::cout<<"ERROR rec nullptr \n";
        }

        if(ptr->data != d.expected)       
        {
            std::cout<<"ERROR rec: "<<ptr->data<<' '<<d.expected<<'\n';
        }
    }
}

void test(const std::vector<testData> &td)
{
    for(auto& d : td)
    {

        auto ptr = getNthToLast(d.data, d.nthToEnd);
        if(ptr == nullptr)
        {
            std::cout<<"ERROR nullptr \n";
        }

        if(ptr->data != d.expected)       
        {
            std::cout<<"ERROR: "<<ptr->data<<' '<<d.expected<<'\n';
        }
    }
}

int main()
{
    List l;
    l.insert(1);
    l.insertAtEnd(2);
    l.insertAtEnd(3);
    l.insertAtEnd(4);

    List l1;
    l1.insert(1);
    l1.insertAtEnd(2);
    l1.insertAtEnd(3);
    l1.insertAtEnd(4);
    l1.insertAtEnd(5);
    l1.insertAtEnd(6);
    l1.insertAtEnd(7);

    List l2;
    l2.insert(1);
    l2.insertAtEnd(2);
    l2.insertAtEnd(3);
    l2.insertAtEnd(4);

    std::vector<testData> td;

    td.emplace_back(testData{l,3,2});
    td.emplace_back(testData{l1,1,7});
    td.emplace_back(testData{l2,4,1});

    test(td);
    testRec(td);
    return 0;
}