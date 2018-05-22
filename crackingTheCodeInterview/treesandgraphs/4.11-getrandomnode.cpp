#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include "treeNodeWithRandom.h"

void test()
{
    std::unique_ptr<TreeNode> root(new TreeNode(20));
    root->insertInOrder(10);
    root->insertInOrder(5);
    root->insertInOrder(2);
    root->insertInOrder(1);
    root->insertInOrder(30);
    root->insertInOrder(35);
    root->insertInOrder(42);
    root->insertInOrder(51);

    if(auto node = root->getRandomNode(); node != nullptr)
    {
        node->InOrderTraversal();  
    }
}

int main(
{
    test();
    return 0;
}
