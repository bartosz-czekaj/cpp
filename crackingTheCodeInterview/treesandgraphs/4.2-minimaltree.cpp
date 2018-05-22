#include <vector>
#include <memory>
#include <iostream>
#include "treeNode.h"


void PostOrderTraversal(const TreeNode &node)
{
    std::cout<<node.value<<'\n';
    if(node.left)
        PostOrderTraversal(*node.left.get());
    if(node.right)
        PostOrderTraversal(*node.right.get());
}

int main()
{
    std::vector<int> data;

    for(auto elem = 1; elem < 101; ++elem)
    {
        data.emplace_back(elem);
    }

    std::unique_ptr<TreeNode> n(std::move(createMinimalBST(data)));

    //PostOrderTraversal(*n.get());
    std::cout<<"Depth: "<<maxDepth(*n.get())<<'\n';
    return 0;
}