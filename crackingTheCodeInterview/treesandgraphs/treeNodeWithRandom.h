struct TreeNode
{
    std::mt19937 m_mt;
    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    int size = 0;

    ~TreeNode()
    {
        //std::cout<<"destruct "<<value<<"\n";
    }

    TreeNode(int data) : m_mt((std::random_device())())
    {
        value = data;
        size = 1;
    }

    int generateTokenType(int upperBound)
    {
        std::uniform_int_distribution<int> dist(0, upperBound);
        return dist(m_mt);
    }

    const TreeNode* getRandomNode()
    {
        auto leftSize = (left) ? left->getSize() : 0;
        auto index = generateTokenType(leftSize);

        if(index < leftSize)
        {
            return left ? left->getRandomNode() : nullptr;
        }
        if(index == leftSize)
        {
            return this;
        }
        return right ? right->getRandomNode() : nullptr;
    }

    void insertInOrder(int data)    
    {
        if(data <= value)
        {
            if(left)
            {
                left->insertInOrder(data);
            }
            else
            {
                left.reset(std::move(new TreeNode(data)));
            }
        }
        else
        {
            if(right)
            {
                right->insertInOrder(data);
            }
            else
            {
                right.reset(std::move(new TreeNode(data)));
            }
        }
        ++size;
    }

    inline int getSize() {return size;}
    inline int getData() {return value;}

    void InOrderTraversal(const TreeNode *node) const
    {
        if(node)
        {
            InOrderTraversal(node->left.get());
            std::cout<<node->value<<'\n';
            InOrderTraversal(node->right.get());
        }
    }

    void InOrderTraversal() const
    {
        InOrderTraversal(this);
    }

    
    void PreOrderTraversal(const TreeNode *node) const
    {
        if(node)
        {
            std::cout<<node->value<<'\n';
            PreOrderTraversal(node->left.get());
            PreOrderTraversal(node->right.get());
        }
    }

    void PreOrderTraversal() const
    {
        PreOrderTraversal(this);
    }

    void PostOrderTraversal(const TreeNode *node) const
    {
        if(node)
        {
            std::cout<<node->value<<'\n';
            PostOrderTraversal(node->left.get());
            PostOrderTraversal(node->right.get());
        }
    }

    void PostOrderTraversal() const
    {
        PostOrderTraversal(this);
    }
};