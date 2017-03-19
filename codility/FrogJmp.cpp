// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
#include <algorithm>
int solution(int X, int Y, int D) {
   return std::ceil(static_cast<double>(Y - X) / static_cast<double>(D));
}