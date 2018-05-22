#include <memory>

template<class T>
class BinarySearchTree
{
    struct TreeNode;
    typedef std::unique_ptr<TreeNode> spTreeNode;
    
    struct TreeNode
    {
        T data;
        spTreeNode  left;
        spTreeNode  right;
        TreeNode(const T & value):data(value),left(nullptr),right(nullptr){}
    };

    spTreeNode root;
    bool insert(spTreeNode &node);
    void print(const spTreeNode&) const ;

    void InOrderTraversal(const spTreeNode &node) const;
    void PreOrderTraversal(const spTreeNode &node) const;
    void PostOrderTraversal(const spTreeNode &node) const;
public:
    
    BinarySearchTree();
    void insert( const T & node);
    void print()const;

    //traversal
    void InOrderTraversal()const;
    void PreOrderTraversal()const;
    void PostOrderTraversal()const;
};

template<class T>
void BinarySearchTree<T>::InOrderTraversal() const
{
    InOrderTraversal(root);
}

template<class T>
void BinarySearchTree<T>::InOrderTraversal(const spTreeNode &node) const
{
    if(node)
    {
        InOrderTraversal(node->left);
        std::cout<<node->data<<'\n';
        InOrderTraversal(node->right);
    }
}

template<class T>
void BinarySearchTree<T>::PreOrderTraversal() const
{
    PreOrderTraversal(root);
}

template<class T>
void BinarySearchTree<T>::PreOrderTraversal(const spTreeNode &node) const
{
    if(node)
    {
        std::cout<<node->data<<'\n';
        PreOrderTraversal(node->left);
        PreOrderTraversal(node->right);
    }
}

template<class T>
void BinarySearchTree<T>::PostOrderTraversal() const
{
    PostOrderTraversal(root);
}

template<class T>
void BinarySearchTree<T>::PostOrderTraversal(const spTreeNode &node) const
{
    if(node)
    {
        std::cout<<node->data<<'\n';
        PostOrderTraversal(node->left);
        PostOrderTraversal(node->right);
    }
}

template<class T>
BinarySearchTree<T>::BinarySearchTree():root(nullptr)
{}

template<class T>
void BinarySearchTree<T>::insert(const T & ref)
{
    std::unique_ptr<TreeNode> node(new TreeNode(ref));
    if (root == nullptr) 
    {
        root = std::move(node);
    }
    else 
    {
        TreeNode* temp = root.get();
        TreeNode* prev = root.get();
        while (temp != nullptr) 
        {
            prev = temp;
            if (temp->data < ref)
                temp = temp->right.get();
            else
                temp = temp->left.get();
        }
        if (prev->data < ref)
        {
            std::cout<<"Right\n";
            prev->right = std::move(node);
        }
        else
        {
            std::cout<<"Left\n";
            prev->left = std::move(node);
        }
    }
}

template<class T>
void BinarySearchTree<T>::print()const
{
    print(root);
}

template<class T>
void BinarySearchTree<T>::print(const std::unique_ptr<TreeNode> &node) const
{
    if(node == nullptr) return;
    print(node->left);
    std::cout << node->data<< std::endl;
    print(node->right);
}