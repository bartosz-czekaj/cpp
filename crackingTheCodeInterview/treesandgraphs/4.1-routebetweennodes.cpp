#include<iostream>
#include <list>
#include <vector>
#include <queue>
#include "graph.h"

struct testData
{
    Graph *g;
    int start;
    int end;
    bool expected;
};

bool search(const Graph &gr, int start, int end)
{
    if(start == end)
    {
        return true;
    }

    std::vector<bool> visited(gr.GetNoOfVertices(), false);
    
    // Create a queue for BFS
    std::queue<int> queueBFS;
 
    // Mark the current node as visited and enqueue it
    visited[start] = true;
    queueBFS.push(start);
 
    while(!queueBFS.empty())
    {
        // Dequeue a vertex from queue and print it
        auto s = queueBFS.front();
        if(end == s)
            return true;
        queueBFS.pop();
 
        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited, 
        // then mark it visited and enqueue it
        for(const auto &el : gr.GetAdj()[s])
        {
            if (!visited[el])
            {
                visited[el] = true;
                queueBFS.push(el);
            }
        }
    }

    return false;
}


void test(const std::vector<testData> &test)
{
    for(const auto &tc : test)
    {
        if(tc.expected != search(*tc.g, tc.start, tc.end))
        {
            std::cout<<"Error: from: "<<tc.start<<" to: "<<tc.end<<" expected: "<<tc.expected<<'\n';
        }
    }
}

int main()
{
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

   
    Graph g1(6);
    g1.addEdge(0,1);
    g1.addEdge(0,2);
    g1.addEdge(2,3);
    g1.addEdge(2,4);
    g1.addEdge(1,5);
    g1.addEdge(3,4);
    g1.addEdge(5,4);
    g1.addEdge(4,1);

    std::vector<testData> testData
    {
        {&g, 1, 2, true},
        {&g, 2, 1, true},
        {&g, 3, 1, false},
        {&g1, 2, 5, true},
        {&g1, 5, 0, false},
        {&g1, 5, 4, true},
        {&g1, 3, 5, true}
    };

    test(testData);

    return 0;
}