// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <stack>
#include <string>

int solution(std::string &S)
{
    int vRet  = 1;
    
    if(S.empty())
        return vRet;
    
    std::stack<bool> vBrackets;
    
    for(const auto &vChar : S)
    {
        if(vChar == ')')
        {
            if(vBrackets.empty())
            {
                vRet = 0;
                break;
            }
            else
                vBrackets.pop();
        }
        else if(vChar == '(')
            vBrackets.push(true);
    }
    
    if(!vBrackets.empty())
        vRet = 0;
    
    return vRet;
}

///

// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <string>
int solution(std::string &S)
{
    int vRet  = 1;
    
    if(S.empty())
        return vRet;
    
    int vBrackets = 0;
    
    for(const auto &vChar : S)
    {
        if( vChar == ')')
            --vBrackets;
        else if(vChar == '(')
            ++vBrackets;
        
        if(vBrackets < 0)
        {
            vRet = 0;
            break;
        }
    }
    
    if(vBrackets >  0)
        vRet = 0;
    
    return vRet;
}