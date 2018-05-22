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
    List data1;
    List data2;
    std::vector<int> expected;
};

Node* getIntersectionNode(Node *l1, Node *l2)
{
    auto getLen = [](Node *head)
    {
        auto len = 0;
        while(head != nullptr)
        {
            head = head->next;
            ++len;
        }

        return len;
    };

    auto lenL1 = getLen(l1);
    auto lenL2 = getLen(l2);

    auto shortL = lenL1 > lenL2 ? l2 : l1;
    auto longL = lenL1 > lenL2 ? l1 : l2;
    
    auto longer = 0;
    while(longer<(std::abs(lenL1-lenL2)))
    {
        longL = longL->next;
        ++longer;
    }

    while(longL!=shortL)
    {
        longL = longL->next;
        shortL = shortL->next;

        if(longL == nullptr || shortL == nullptr)
        {
            return nullptr;
        }
    }

    return shortL;
}

std::vector<int> getIntersection(Node *l1, Node *l2)
{
    auto in = getIntersectionNode(l1,l2);

    std::vector<int> data;

    while(in != nullptr)
    {
        data.emplace_back(in->data);
        in = in->next;
    }

    return data;
}

void test(const std::vector<testData> &td)
{
    auto idx = 1;
    for(auto& d : td)
    {
        auto ret = getIntersection(d.data1.head,d.data2.head);
        if(ret != d.expected)
        {
            std::cout<<"ERROR nb: "<<idx<<'\n';
            for(const auto &r : ret)
            {
                std::cout<<r<<", ";
            }
            std::cout<<'\n';
        }
        ++idx;
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

    auto getTail = [](List &l)
    {
        auto current = l.head;
        while(current != nullptr)
        {
            if(current->next == nullptr)
            {
                break;
            }
            current = current->next;
        }

        return current;
    };
    getTail(l)->next = l1.head;    

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
    l4.insertAtEnd(112);
    l4.insertAtEnd(113);
    l4.insertAtEnd(114);
    l4.insertAtEnd(115);
    l4.insertAtEnd(116);
    l4.insertAtEnd(117);

    List l5;
    l5.insert(211);
    l5.insertAtEnd(1212);
    l5.insertAtEnd(1213);
    l5.insertAtEnd(1214);
    l5.insertAtEnd(12214);
    l5.insertAtEnd(13214);
    l5.insertAtEnd(14214);
    l5.insertAtEnd(15214);
    l5.insertAtEnd(15614);

    getTail(l4)->next = l5.head->next->next->next->next;

    std::vector<testData> td;

    td.emplace_back(testData{l,l1, std::vector<int>{1,2,3,4,3,2,1}});
    td.emplace_back(testData{l2,l3, std::vector<int>()});
    td.emplace_back(testData{l4,l5, std::vector<int>{12214,13214,14214,15214,15614}});
   
    test(td);

    return 0;
}