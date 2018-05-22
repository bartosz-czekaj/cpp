#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include "list.h"

struct testData
{
    List data;
    int pivot;
};

Node* partition(Node *head, int pivot)
{
    auto *newHead = head;
    auto *tail = head;
    auto *current = head;

    while(current !=nullptr)
    {
        auto *vNext = current->next;
        if(current->data < pivot)
        {
            current->next = newHead;
            newHead = current;
        }
        else
        {
            tail->next = current;
            tail = current;
        }

        current = vNext;
    }

    tail->next = nullptr;
}

void test(const std::vector<testData> &td)
{
    for(auto& d : td)
    {

        auto head = partition(d.data.head, d.pivot);
        if(head == nullptr)
        {
            std::cout<<"ERROR nullptr \n";
            return;
        }
        auto rubikon = false;
        while(head != nullptr)
        {
            if(head->data >= d.pivot)
                rubikon = true;
            if(head->data < d.pivot && rubikon)
            {
                std::cout<<"ERROR "<<head->data<<"\n";
                return;
            }

            head = head->next;
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

    List l3;
    l3.insert(11);
    l3.insertAtEnd(222);
    l3.insertAtEnd(31);
    l3.insertAtEnd(14);
    l3.insertAtEnd(25);
    l3.insertAtEnd(36);
    l3.insertAtEnd(47);


    std::vector<testData> td;

    td.emplace_back(testData{l,2});
    td.emplace_back(testData{l1,4});
    td.emplace_back(testData{l2,1});
    td.emplace_back(testData{l3,224});

   
    test(td);


    return 0;
}