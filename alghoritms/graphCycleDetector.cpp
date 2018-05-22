#include <iostream>
#include <vector>
#include <array>

bool Helper(const std::vector<std::vector<int>> &graph, int idx, std::vector<bool> &visited, std::vector<bool> &recStack)
{
    visited[idx] = true;
    recStack[idx] = true;
    
    for(const auto &elem : graph[idx])
    {
        if(recStack[elem])
            return true;

        if (elem == idx)
            return true;

        if(!visited[elem])
        {
            if(Helper(graph, elem, visited, recStack))
                return true;
        }
    }

    recStack[idx] = false;

    return false;
}

bool IsCyclic(const std::vector<std::vector<int>> &graph)
{
    std::vector<bool> visited; 
    std::vector<bool> recStack; 

    visited.reserve(graph.size());
    recStack.reserve(graph.size());

    
    size_t idx = 0;
    for(const auto &elems : graph) 
    {
        if(visited[idx] == false)
        {
            if(Helper(graph, idx, visited, recStack))
                return true;
        }

        ++idx;
    }

    return false;
}

int main()
{
    std::vector<std::vector<int>> graph{{1}, {2,3}, {3}, {3}} ;

    /*graph.emplace_back(std::vector<int>{1});
    graph.emplace_back(std::vector<int>{2});
    graph.emplace_back(std::vector<int>{3});
    graph.emplace_back(std::vector<int>());*/

    std::cout<<(IsCyclic(graph)? "true" : "false")<<'\n';

    return 0;
}