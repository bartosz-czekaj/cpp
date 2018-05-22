#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

char Encrypt(const char &ch, int move)
{
    char a = std::isupper(ch)?'A':'a';
    return a + ((ch - a + move)%26);
}

int main()
{
    int n;
    cin >> n;
    string s;
    cin >> s;
    int k;
    cin >> k;
    
    
    for(auto &c : s)
    {
       std::cout<<(std::isalpha(ch) ? Encrypt(c, k) : c );
    }
    
    return 0;
}
