#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include <list>
#include "treeNode.h"

struct TestData
{
    const TreeNode *node = nullptr;
    bool isBST;
};

bool isBST2(const TreeNode* root)
{
    static const TreeNode *prev = nullptr;
     
    // traverse the tree in inorder fashion and keep track of prev node
    if (root)
    {
        if (!isBST2(root->left.get()))
          return false;
 
        // Allows only distinct valued nodes 
        if (prev != nullptr && root->value <= prev->value)
          return false;
 
        prev = root;
 
        return isBST2(root->right.get());
    }
 
    return true;
}

bool isBST(const TreeNode* const root, const  TreeNode* const left=nullptr, const TreeNode* const right=nullptr)
{
    // Base condition
    if (root == nullptr)
        return true;
 
    // if left node exist that check it has
    // correct data or not
    if (left != nullptr and root->value < left->value)
        return false;
 
    // if right node exist that check it has
    // correct data or not
    if (right != nullptr and root->value > right->value)
        return false;
 
    // check recursively for every node.
    return isBST(root->left.get(), left, root) && isBST(root->right.get(), root, right);
}

void test(const std::vector<TestData> &tds)
{
    auto item = 1;
    for(const auto &td : tds)
    {
       if(isBST(td.node) != td.isBST)      
       {
           std::cout<<"Error (BST) expected for ["<<item<<"]: "<<td.isBST<<'\n';
       }
       if(isBST2(td.node) != td.isBST)     
       {
           std::cout<<"Error (BST2) expected for ["<<item<<"]: "<<td.isBST<<'\n';
       }
       ++item;
    }
}

int main()
{
    auto t1 = new TreeNode();
    auto t12 = new TreeNode();
    auto t13 = new TreeNode();
    auto t121 = new TreeNode();
    auto t122 = new TreeNode();

    t122->value = 3;
    t121->value = 1;
    t12->value = 2;
    t13->value = 5;
    t1->value = 4;

    t12->right.reset(std::move(t122));
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

    auto at1 = new TreeNode();
    auto at12 = new TreeNode();
    auto at13 = new TreeNode();
    auto at121 = new TreeNode();
    auto at122 = new TreeNode();

    at122->value = 4;
    at121->value = 1;
    at12->value = 2;
    at13->value = 5;
    at1->value = 3;

    at12->right.reset(std::move(at122));
    at12->left.reset(std::move(at121));
    at1->left.reset(std::move(at12));
    at1->right.reset(std::move(at13));

    std::unique_ptr<TreeNode> atn(std::move(at1));

    std::vector<TestData> td
    {
        {tn.get(), true},
        {n.get(), true},
        {atn.get(), false}
    };

    test(td);
    return 0;
}