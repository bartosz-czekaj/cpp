#include <deque>
#include <memory>
#include <vector>
#include <iostream>
#include "treeNode.h"

bool findPath(const TreeNode *root, std::deque<int> &path, int expected)
{
    if (root == nullptr) 
    {
        return false;
    }
 
    path.emplace_front(root->value);
 
    if (root->value == expected)
    {
        return true;
    }
    
    if ( (root->left && findPath(root->left.get(), path, expected)) || (root->right && findPath(root->right.get(), path, expected)) )
    {
        return true;
    }
    
    path.pop_front();
    return false;
}

bool findLCA(const TreeNode *root, int child1, int child2, int &parent)
{
    if(!root)
    {
        std::cout<<"NOROOT\n";
        return false;
    }
    std::deque<int> path1;
    std::deque<int> path2;
 
    if ( !findPath(root, path1, child1) || !findPath(root, path2,child2))
    {
        return false;
    }

    auto pathWalker = [](const std::deque<int> &path)
    {
        for(const auto &elem : path)
        {
            std::cout<<elem<<' ';
        }
        std::cout<<'\n';
    };

    //pathWalker(path1);
    //pathWalker(path2);
 
    auto index = 1;
    while(index < path1.size() && index < path2.size())
    {
       if(path1.size()-index < 0 || path2.size()-index < 0)
       {
           parent = path1[path1.size()-index];
           break;
       }

       if (path1[path1.size()-index] != path2[path2.size()-index])
       {
           parent = path1[path1.size()-(index-1)];
           break;
       }
       ++index;
    }

    return true;
}

struct testData
{
    const TreeNode *root;
    int firstChild;
    int secondChild;
    int commonAncestor;
    bool exists;
};

void test(const std::vector<testData> &testData)
{
    for(const auto &testCase : testData)
    {
        auto ret = 0;
        auto ancestorExists = findLCA(testCase.root , testCase.firstChild, testCase.secondChild, ret);

        if(ancestorExists != testCase.exists)
        {
            std::cout<<"Error expected: "<<testCase.exists<<" given: "<<ancestorExists<<'\n';
            continue;
        }

        if(ret != testCase.commonAncestor)
        {
            std::cout<<"Error expected: "<<testCase.commonAncestor<<" given: "<<ret<<'\n';
        }
    }
}


int main()
{
    std::vector<int> data;
    for(auto elem = 1; elem < 10; ++elem)
    {
        data.emplace_back(elem);
    }

    std::unique_ptr<TreeNode> n(std::move(createMinimalBST(data)));

    std::vector<testData> testData
    {
        {n.get(), 1, 2, 2, true},
        {n.get(), 1, 9, 5, true}
    };

    test(testData);

    return 0;
}