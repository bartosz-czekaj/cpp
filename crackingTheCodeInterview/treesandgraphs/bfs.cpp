// Program to print BFS traversal from a given
// source vertex. BFS(int s) traverses vertices 
// reachable from s.
#include<iostream>
#include <list>
#include <vector>
#include <queue>

 
// This class represents a directed graph using
// adjacency list representation
class Graph
{
    int noOfVertices;    // No. of vertices
 
    // Pointer to an array containing adjacency
    // lists
    std::vector<std::list<int>> adj;   
public:
    Graph(int V);  // Constructor
 
    // function to add an edge to graph
    void addEdge(int v, int w); 
 
    // prints BFS traversal from a given source s
    void BFS(int s);  
};
 
Graph::Graph(int V)
{
    noOfVertices = V;
    adj.resize(noOfVertices);
}
 
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}
 
void Graph::BFS(int s)
{
    // Mark all the vertices as not visited
    std::vector<bool> visited(noOfVertices, false);
    
    // Create a queue for BFS
    std::queue<int> queueBFS;
 
    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queueBFS.push(s);
 
    while(!queueBFS.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queueBFS.front();
        std::cout << s << " ";
        queueBFS.pop();
 
        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited, 
        // then mark it visited and enqueue it
        for(const auto &el : adj[s])
        {
            if (!visited[el])
            {
                visited[el] = true;
                queueBFS.push(el);
            }
        }
    }
}
 
// Driver program to test methods of graph class
int main()
{
    // Create a graph given in the above diagram
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);
 
    std::cout << "Following is Breadth First Traversal "
         << "(starting from vertex 2) \n";
    g.BFS(2);
    std::cout<<'\n';
 
    return 0;
}