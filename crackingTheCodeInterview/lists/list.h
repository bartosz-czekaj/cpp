class Node
{
public:
    int data;
    Node *next;
};

class List
{
    public:
    Node *head;
    Node *tail;
    Node *temp;
    bool isEmpty()
    {
        return head == NULL;
    }

    List()
    {
        head = NULL;
        tail = NULL;
    }

    void insert(int x)
    {
        temp = new Node;
        temp->data = x;
        if(isEmpty())
        {
            temp->next = NULL;
            tail = temp;
        }
        else
            temp->next = head;
        head = temp;
    }

    void insertAtEnd(int x)
    {
        temp = new Node;
        temp->data = x;
        temp->next = NULL;
        if(isEmpty())
        {
            head = temp;
            tail = temp;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }
    }

    void remove(int x)
    {
        temp = head;
        Node *prev;
        while(temp->next != NULL && temp->data != x)
        {
            prev = temp;
            temp = temp->next;
        }
        if(temp->data == x)
        {
            prev->next = temp->next;
            delete temp;
        }
        else if(temp->next == NULL)
        {
            std::cout << "Error: Number Not found..." << std::endl;
        }
    }

    void find(int x)
    {
        int i;
        for(i=1, temp = head;temp->next != NULL && temp->data != x;temp = temp->next, i++);
        if(temp->data == x)
        {
            std::cout << "Found at position:" << i << std::endl;
        }
        else if(temp->next == NULL)
        {
            std::cout << "Error: Number Not found..." << std::endl;
        }
    }

    void display()
    {
        if(!isEmpty())
        {
            for(temp = head; temp != NULL; temp=temp->next)
                std::cout << temp->data << " ";
            std::cout << std::endl;
        }
        else
        {
            std::cout << "List is Empty!" << std::endl;
        }
    }
};
