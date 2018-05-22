bool has_cycle(Node* head) {
    if (head == nullptr)
        return false;
    
    Node *slow = head;
    Node *fast = head->next;
    
    while(slow != nullptr && fast != nullptr)
    {
        if(slow == fast)
            return true;
        
        if(fast == nullptr || fast->next == nullptr)
            return false;
        
        slow = slow->next;
        fast = fast->next->next;
    }    
    
   
    
    return false;
  