#include <memory>
#include <iostream>
#include <vector>

struct TreeNode
{
    int data;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;
    TreeNode *parent = nullptr;

    ~TreeNode()
    {
       //std::cout<<"destruct "<<data<<'\n';
    }
};

void InOrderTraversal(const TreeNode *const node)
{
    if(node)
    {
        InOrderTraversal(node->left.get());
        std::cout<<node->data<<' '<<"( "<<node->parent<<") ";
        InOrderTraversal(node->right.get());
    }
}

struct testData
{
    TreeNode *root;
    TreeNode *reference;
    int expected;
    bool exists;
};


TreeNode* GetSuccessor(TreeNode* root, TreeNode* current) 
{
    if(current == nullptr)
    {
        return nullptr;
    }

    if(current->right)
    {
        auto tmp = current->right.get();
        while(tmp->left.get())
        {
            tmp = tmp->left.get();
        }
        return tmp;
    }
   
    auto parent = current->parent;
    auto tmp = current;
    while(parent && tmp == parent->right.get())
    {
        tmp = parent;
        parent = parent->parent;
    }
    return parent;
}


TreeNode* newNode(int data)
{
 TreeNode* node = new TreeNode();
  node->data = data;
  node->parent = nullptr;
  node->right.reset(); 
  node->left.reset();
  return node;
}

void insert(std::unique_ptr<TreeNode> &root, int data)
{
    std::unique_ptr<TreeNode> node(newNode(data));
    if (!root) 
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
            if (temp->data < data)
                temp = temp->right.get();
            else
                temp = temp->left.get();
        }
        if (prev->data < data)
        {
            prev->right = std::move(node);
            prev->right->parent = prev;
        }
        else
        {
            prev->left = std::move(node);
            prev->left->parent = prev;       
        }
    }
}


void test(const std::vector<testData> &data)
{
    for(const auto &d : data)
    {
        auto gs = GetSuccessor(d.root, d.reference);

        if(gs != nullptr && !d.exists)
        {
            std::cout<<"ERROR schould not exist\n";
            continue;
        }

        if(gs == nullptr && d.exists)
        {
            std::cout<<"ERROR schould exist\n";
            continue;
        }

        if(gs != nullptr)
        {
            if(gs->data != d.expected)
            {
                std::cout<<"error expected: "<<d.reference<<" given: "<<gs->data<<'\n';
            }
        }
    }
}

int main()
{
  std::unique_ptr<TreeNode> root;

  insert(root, 20);
  insert(root, 8);
  insert(root, 22);
  insert(root, 4);
  insert(root, 12);
  insert(root, 10);
  insert(root, 14);

  std::vector<testData> testData
  {
      {root.get(), root->left->left.get(), 8, true},
      {root.get(), root->left.get(), 10, true},
      {root.get(), root->left->right.get(), 14, true},
      {root.get(), root.get(), 22, true},
      {root.get(), root->right.get(), -1, false},
  };
  
  test(testData);
  
  return 0;
}