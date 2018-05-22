// Program to print BFS traversal from a given
// source vertex. BFS(int s) traverses vertices 
// reachable from s.


 
// This class represents a directed graph using
// adjacency list representation
class Graph
{
    int noOfVertices;    // No. of vertices
 
    // Pointer to an array containing adjacency
    // lists
    std::vector<std::list<int>> adj; 
     void DFSUtil(int v, std::vector<bool> &visited);  
public:
    inline int GetNoOfVertices() const
    {
        return noOfVertices;
    }

    const std::vector<std::list<int>>& GetAdj() const
    {
        return adj;
    }

    Graph(int V);  // Constructor
 
    // function to add an edge to graph
    void addEdge(int v, int w); 
 
    // prints BFS traversal from a given source s
    void BFS(int s);  
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