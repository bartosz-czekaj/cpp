#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "treeNode.h"

bool containsTree(TreeNode *t1, TreeNode *t2)
{
    const auto getOrderString =[](const auto &self, TreeNode *node, std::string &pathString)
    {
        if(node == nullptr)
        {
            pathString += 'X';
            return;
        }

        pathString += std::to_string(node->value);
        self(self, node->left.get(), pathString);
        self(self, node->right.get(), pathString);
    };

    std::string pathString1;
    std::string pathString2;
    getOrderString(getOrderString, t1, pathString1);
    getOrderString(getOrderString, t2, pathString2);

    return pathString1.find(pathString2) != std::string::npos;
}

struct testData
{
    TreeNode *root1;
    TreeNode *root2;
    bool subtree;
};

void test(const std::vector<testData> &tests)
{
    for(const auto &testCase : tests)
    {
        auto isSubtree = containsTree(testCase.root1, testCase.root2);
        if(isSubtree != testCase.subtree)
        {
            std::cout<<"Error expected: "<<testCase.subtree<<" given: "<<isSubtree<<'\n';
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


    auto d1 = new TreeNode();
    auto d2 = new TreeNode();
    auto d3 = new TreeNode();
    auto d4 = new TreeNode();
    auto d5 = new TreeNode();
   
    d1->value = 1;
    d2->value = 2;
    d3->value = 3;
    d4->value = 4;
    d5->value = 5;

    d2->left.reset(std::move(d3));
    d2->right.reset(std::move(d1));

    d4->left.reset(std::move(d2));
    d4->right.reset(std::move(d5));

    auto e1 = new TreeNode();
    auto e2 = new TreeNode();
    auto e3 = new TreeNode();
    auto e4 = new TreeNode();
    auto e5 = new TreeNode();
   
    e1->value = 1;
    e2->value = 2;
    e3->value = 3;
    e4->value = 4;
    e5->value = 5;

    e2->left.reset(std::move(e1));
    e2->right.reset(std::move(e3));

    e4->left.reset(std::move(e2));
    e4->right.reset(std::move(e5));

    std::unique_ptr<TreeNode> pt1(std::move(t1));
    std::unique_ptr<TreeNode> pt2(std::move(c1));
    std::unique_ptr<TreeNode> pt3(std::move(d4));
    std::unique_ptr<TreeNode> pt4(std::move(e4));

    std::vector<testData> tests
    {
        {pt1.get(), pt1.get(), true},
        {pt2.get(), pt2.get(), true},
        {pt1.get(), pt2.get(), false},
        {pt3.get(), pt1.get(), false},
        {pt4.get(), pt1.get(), true}
    };

    test(tests);
    return 0;
}