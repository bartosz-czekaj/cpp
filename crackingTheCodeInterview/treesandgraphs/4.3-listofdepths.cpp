#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <list>
#include "treeNode.h"

void collectListOfDepth(const TreeNode &node, int depth, std::unordered_map<int, std::list<int> > &listOfDepth) 
{
    listOfDepth[depth].emplace_back(node.value);

    if(node.left)
        collectListOfDepth(*node.left.get(), (depth+1), listOfDepth);
    if(node.right)    
        collectListOfDepth(*node.right.get(), (depth+1), listOfDepth);
} 


void test(int min, int max)
{

    std::vector<int> data;

    for(auto elem = min; elem < max; ++elem)
    {
        data.emplace_back(elem);
    }

    std::unique_ptr<TreeNode> n(std::move(createMinimalBST(data)));

    std::unordered_map<int, std::list<int>> listOfDepth;
    collectListOfDepth(*n.get(), 0, listOfDepth);

    auto mD = maxDepth(*n.get());

    if(mD != listOfDepth.size())
    {
        std::cout<<"Error expected: "<<mD<<" given: "<< listOfDepth.size()<<'\n';
    }
}

int main()
{
    test(1,10);
    test(100,10000);
    test(22,56);
    test(10,20);
    return 0;
}
