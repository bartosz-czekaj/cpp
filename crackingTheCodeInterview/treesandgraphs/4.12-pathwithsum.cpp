#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <unordered_map>
#include "treeNodeWithRandom.h"

void incrementHashTable(std::unordered_map<int, int> &pathCount, int key, int delta)
{
    auto newCount = 0;
    auto element = pathCount.find(key);
    if(element != pathCount.end()) 
    {
        newCount = pathCount[key];
    }

    newCount += delta;

    if(newCount)
    {
        pathCount[key]=newCount;
    }
    else
    {
        if(element!= pathCount.end())
        {
            pathCount.erase(element);
        }
    }
}

int countPathsWithSum(TreeNode *node, int targetSum, int runningSum, std::unordered_map<int, int> &pathCount)
{
    if(node == nullptr)
    {
        return 0;
    }

    runningSum += node->value;
    auto sum = runningSum - targetSum;
    auto totalPaths = 0;
    if(pathCount.find(sum)!= pathCount.end()) 
    {
        totalPaths = pathCount[sum];
    }

    if(runningSum == targetSum)
    {
        ++totalPaths;
    }

    incrementHashTable(pathCount, runningSum, 1);
    totalPaths += countPathsWithSum(node->left.get(), targetSum, runningSum, pathCount);
    totalPaths += countPathsWithSum(node->right.get(), targetSum, runningSum, pathCount);
    incrementHashTable(pathCount, runningSum, -1);

    return totalPaths;
}

int countPathsWithSum(TreeNode *root, int targetSum)
{
    std::unordered_map<int, int> pathCount;
    return countPathsWithSum(root, targetSum, 0, pathCount);
}

struct testData
{
    TreeNode *root;
    int sum;
    int expected;
};

void test(std::vector<testData> &testData)
{
    for(const auto &testCase : testData)
    {
        auto given = countPathsWithSum(testCase.root, testCase.sum);
        if(given != testCase.expected)
        {
            std::cout<<"Error expected: "<<testCase.expected<<" given: "<<given<<'\n';
        }
    }
}

int main()
{
    auto a1 = new TreeNode(2);
    auto a2 = new TreeNode(2);
    auto a3 = new TreeNode(2);
    auto a4 = new TreeNode(2);
    auto a5 = new TreeNode(2);
   
    a2->left.reset(std::move(a1));
    a2->right.reset(std::move(a3));

    a4->left.reset(std::move(a2));
    a4->right.reset(std::move(a5));

    auto b1 = new TreeNode(2);
    auto b2 = new TreeNode(3);
    auto b3 = new TreeNode(3);
    auto b4 = new TreeNode(2);
    auto b5 = new TreeNode(3);
   
    b2->left.reset(std::move(b1));
    b2->right.reset(std::move(b3));

    b4->left.reset(std::move(b2));
    b4->right.reset(std::move(b5));

    auto e1 = new TreeNode(2);
    auto e2 = new TreeNode(3);
    auto e3 = new TreeNode(2);
    auto e4 = new TreeNode(3);
    auto e5 = new TreeNode(2);
   
    e2->left.reset(std::move(e1));
    e2->right.reset(std::move(e3));

    e4->left.reset(std::move(e2));
    e4->right.reset(std::move(e5));

    std::unique_ptr<TreeNode> root1(std::move(e4));
    std::unique_ptr<TreeNode> root2(std::move(a4));
    std::unique_ptr<TreeNode> root3(std::move(b4));

    std::vector<testData> data
    {
        {root1.get(), 5, 3},
        {root2.get(), 4, 4},
        {root3.get(), 6, 1},
        {root3.get(), 7, 1}
    };

    test(data);

    return 0;
}