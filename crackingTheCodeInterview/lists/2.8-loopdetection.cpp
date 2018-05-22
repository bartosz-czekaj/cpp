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
    Node* start;
};


Node* findLoopBegin(Node* node)
{
    auto slow = node;
    auto fast = node;
    Node *collision = nullptr;

    while(fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(slow == fast)
        {
            collision = slow;
            break;
        }
    }

    if(collision == nullptr)
    {
        return collision;
    }

    slow = node;
    while(slow != fast)
    {
        slow = slow->next;
        fast = fast->next;
    }
}


void test(const std::vector<testData> &td)
{
    auto idx = 1;
    for(auto& d : td)
    {
        auto ret = findLoopBegin(d.data1.head);
        if(ret != d.start)
        {
            std::cout<<"ERROR nb: "<<idx<<" ex: "<<d.start<<" ret: "<<ret<<'\n';
            
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

    l1.tail->next = l1.head->next->next;

    List l2;
    l2.insert(1);
    l2.insertAtEnd(2);
    l2.insertAtEnd(1);
    l2.insertAtEnd(2);
    l2.tail->next = l2.head;

    List l3;
    l3.insert(11);
    l3.insertAtEnd(222);
    l3.insertAtEnd(31);
    l3.insertAtEnd(14);
    l3.insertAtEnd(25);
    l3.insertAtEnd(36);
    l3.insertAtEnd(47);

     l3.tail->next = l3.head->next->next->next->next->next->next;

    List l4;
    l4.insert(11);
    l4.insertAtEnd(112);
    l4.insertAtEnd(113);
    l4.insertAtEnd(114);
    l4.insertAtEnd(115);
    l4.insertAtEnd(116);
    l4.insertAtEnd(117);

    l4.tail->next = l4.head->next;

    std::vector<testData> td;

    td.emplace_back(testData{l, nullptr});
    td.emplace_back(testData{l1, l1.head->next->next});
    td.emplace_back(testData{l2, l2.head});
    td.emplace_back(testData{l3, l3.head->next->next->next->next->next->next});
    td.emplace_back(testData{l4, l4.head->next});

    test(td);
    return 0;
}