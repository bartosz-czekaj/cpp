// C++ program to print DFS traversal from
// a given vertex in a  given graph
#include<iostream>
#include<list>
#include<vector>
 
// Graph class represents a directed graph
// using adjacency list representation
class Graph
{
    int noOfVertices;    // No. of vertices
 
    // Pointer to an array containing
    // adjacency lists
    std::vector<std::list<int>> adj;
 
    // A recursive function used by DFS
    void DFSUtil(int v, std::vector<bool> &visited);
public:
    Graph(int V);   // Constructor
 
    // function to add an edge to graph
    void addEdge(int v, int w);
 
    // DFS traversal of the vertices
    // reachable from v
    void DFS(int v);
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
 
void Graph::DFSUtil(int v, std::vector<bool> &visited)
{
    // Mark the current node as visited and
    // print it
    visited[v] = true;
    std::cout << v << " ";
 
    // Recur for all the vertices adjacent
    // to this vertex

    for(const auto &el : adj[v])
    {
        if (!visited[el])
        {
            DFSUtil(el, visited);
        }
    }
}
 
// DFS traversal of the vertices reachable from v.
// It uses recursive DFSUtil()
void Graph::DFS(int v)
{
    // Mark all the vertices as not visited
    std::vector<bool> visited(noOfVertices, false);
 
    // Call the recursive helper function
    // to print DFS traversal
    DFSUtil(v, visited);
}
 
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
 
    std::cout << "Following is Depth First Traversal (starting from vertex 2) \n";
    g.DFS(2);
    std::cout << '\n';
 
    return 0;
}