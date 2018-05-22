#include <vector>
#include <algorithm>
#include <iterator>
#include <deque>
#include <memory>
#include <iostream>
#include "treeNode.h"

void waveLists(std::deque<int> first, 
               std::deque<int> second,
               std::deque<std::deque<int>> &results,
               std::deque<int> &prefix)
{
    if(first.empty() || second.empty())
    {
        std::deque<int> result;
        
        result.insert(result.end(), prefix.begin(), prefix.end());
        result.insert(result.end(), first.begin(), first.end());
        result.insert(result.end(), second.begin(), second.end());
        results.emplace_back(result);
        return;
    }
   
    auto headFirst = first.front();
    first.pop_front();
    prefix.emplace_back(headFirst);
    waveLists(first, second, results, prefix);
    prefix.pop_back();
    first.emplace_front(headFirst);
 
    auto headSecond = second.front();
    second.pop_front();
    prefix.emplace_back(headSecond);
    waveLists(first, second, results, prefix);
    prefix.pop_back();
    second.emplace_front(headSecond);
}

std::deque<std::deque<int>> allSequences(TreeNode *node)
{
    std::deque<std::deque<int>> result;
    
    if(node == nullptr)
    {
        result.push_back(std::deque<int>());
        return result;
    }
    std::deque<int> prefix;
    prefix.emplace_back(node->value);
    
    auto leftSeq = allSequences(node->left.get());
    auto rightSeq = allSequences(node->right.get());

    for(auto &left : leftSeq)
    {
        for(auto &right : rightSeq)
        {
            waveLists(left, right, result, prefix);
        }
    }
    return result;
}

struct testData
{
    TreeNode *root;
    int expected_size = 0;
};

void test(const std::vector<testData> &tests)
{
    for(const auto &testCase : tests)
    {
         auto size = allSequences(testCase.root).size();
         if(size != testCase.expected_size)
         {
             std::cout<<"ERROR expected: "<<testCase.expected_size<<" given: "<<size<<'\n';
         }
    }
}

int main()
{
    auto t1 = new TreeNode();
    auto t12 = new TreeNode();
    auto t13 = new TreeNode();
   
    t12->value = 3;
    t13->value = 1;
    t1->value = 2;

    t1->left.reset(std::move(t13));
    t1->right.reset(std::move(t12));

    
    auto c1 = new TreeNode();
    auto c2 = new TreeNode();
    auto c6 = new TreeNode();
    auto c5 = new TreeNode();
    auto c7 = new TreeNode();
   
    c1->value = 3;
    c2->value = 2;
    c6->value = 6;
    c5->value = 5;
    c7->value = 7;

    c6->left.reset(std::move(c5));
    c6->right.reset(std::move(c7));

    c1->left.reset(std::move(c2));
    c1->right.reset(std::move(c6));

    std::unique_ptr<TreeNode> pt1(std::move(t1));
    std::unique_ptr<TreeNode> pt2(std::move(c1));

    std::vector<testData> tests
    {
        {pt1.get(), 2},
        {pt2.get(), 8}
    };

    test(tests);
    return 0;
}