#include <iostream>
#include "tree.h"

int main()
{
    BinarySearchTree<int> tree;

    for(auto elem = 1; elem < 21; elem+=2)
    {
        tree.insert(elem);
    }
    for(auto elem = 2; elem < 20; elem+=2)
    {
        tree.insert(elem);
    }
    tree.InOrderTraversal();
    tree.PreOrderTraversal();

    return 0;
}