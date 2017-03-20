// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.

// cout << "this is a debug message" << endl;
#include <stack>

int solution(std::string &S)
{
    std::stack<int> vBrackets;

    for(const auto &vChar: S)
    {
        int vAddIndex = 0;

        switch (vChar)
        {
        case '(':
        case '{':
        case '[':
            vBrackets.push(vChar);
            continue;
        case ')':
            vAddIndex = 1;
            break;
        case '}':
        case ']':
            vAddIndex = 2;
            break;
        default:
            continue;
        }

        if (vAddIndex && vBrackets.empty())
            return 0;

        if (vAddIndex)
        {
            const int kValue = vBrackets.top();
            if(kValue + vAddIndex == vChar)
                vBrackets.pop();
            else
                return 0;

        }
    }

    return vBrackets.empty() ? 1 : 0;
}