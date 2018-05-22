#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include <list>
#include "treeNode.h"

struct TestData
{
    const TreeNode *node = nullptr;
    bool isBalanced;
};

int height(const TreeNode *node)
{
   /* base case tree is empty */
   if(node == nullptr)
       return 0;

   return 1 + std::max(height(node->left.get()), height(node->right.get()));
} 

bool isBalanced(const TreeNode *node)
{
    auto lh = 0;
    auto rh = 0;

    /* If tree is empty then return true */
   if(node == nullptr)
    return 1; 
 
   /* Get the height of left and right sub trees */
   lh = height(node->left.get());
   rh = height(node->right.get());
 
   if(std::abs(lh-rh) <= 1 && isBalanced(node->left.get()) && isBalanced(node->right.get()))
   {
     return true;
   }
   return false;
}

void test(const std::vector<TestData> &tds)
{
    for(const auto &td : tds)
    {
       if(isBalanced(td.node) != td.isBalanced)     
       {
           std::cout<<"Error expected: "<<isBalanced<<'\n';
       }
    }
}

int main()
{
    auto t1 = new TreeNode();
    auto t12 = new TreeNode();
    auto t13 = new TreeNode();
    auto t121 = new TreeNode();
    auto t1211 = new TreeNode();

    t1211->value = 1211;
    t121->value = 121;
    t12->value = 12;
    t13->value = 13;
    t1->value = 1;

    t121->left.reset(std::move(t1211));
    t12->left.reset(std::move(t121));
    t1->left.reset(std::move(t12));
    t1->right.reset(std::move(t13));

    std::unique_ptr<TreeNode> tn(std::move(t1));

    std::vector<int> data;
    for(auto elem = 11; elem < 1111; ++elem)
    {
        data.emplace_back(elem);
    }

    std::unique_ptr<TreeNode> n(std::move(createMinimalBST(data)));

    std::vector<TestData> td
    {
        {tn.get(), false},
        {n.get(), true}
    };

    test(td);
    return 0;
}