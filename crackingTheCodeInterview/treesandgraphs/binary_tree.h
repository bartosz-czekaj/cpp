#include <variant>
#include <memory>
#include <iostream>

template<typename Leaf>
struct binary_tree;

template<typename Leaf>
struct binary_tree_branch
{
    std::shared_ptr<binary_tree<Leaf>> left;
    std::shared_ptr<binary_tree<Leaf>> right;
};

template<typename Leaf>
struct binary_tree
{
    std::variant<Leaf, binary_tree_branch<Leaf>> value;
};

int main()
{
    binary_tree<int> bt;
    return 0;
}
