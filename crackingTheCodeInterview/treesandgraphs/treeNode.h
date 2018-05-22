struct TreeNode
{
    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;

    ~TreeNode()
    {
        //std::cout<<"destruct "<<value<<"\n";
    }
};

TreeNode *createMinimalBST(const std::vector<int> &data, int start, int end)
{
    if(start > end)
    {
        return nullptr;
    }

    auto mid = (start + end) / 2;
    auto n = new TreeNode();

    n->value = data[mid];
    n->left.reset(std::move(createMinimalBST(data, start, mid -1)));
    n->right.reset(std::move(createMinimalBST(data, mid+1, end)));
    
    return n;
}

TreeNode *createMinimalBST(const std::vector<int> &data)
{
    return createMinimalBST(data, 0, data.size()-1);
}

int maxDepth(const TreeNode &node) 
{
    auto lDepth = 0;
    auto rDepth = 0;

    if(node.left)
        lDepth = maxDepth(*node.left.get());
    if(node.right)    
        rDepth = maxDepth(*node.right.get());
 
    if (lDepth > rDepth) 
        return(lDepth+1);
    else 
        return(rDepth+1);
} 

void insert(std::unique_ptr<TreeNode> &root, int data)
{
    auto newNode = [](int data)
    {
        TreeNode* node = new TreeNode();
        node->value = data;
        node->right.reset(); 
        node->left.reset();
        return node;
    };

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
            if (temp->value < data)
                temp = temp->right.get();
            else
                temp = temp->left.get();
        }
        if (prev->value < data)
        {
            prev->right = std::move(node);
        }
        else
        {
            prev->left = std::move(node);
        }
    }
}