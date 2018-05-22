#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

class Project
{
    std::vector<std::shared_ptr<Project>> children;
    std::unordered_map<char, std::shared_ptr<Project>> map;
    char name;
    int dependencies = 0;
public:
    Project(char n) : name (n) {}
    Project() = default;

    void addNeighbor(std::shared_ptr<Project> &node)
    {
        if(map.find(node->getName()) == map.end())
        {
            children.emplace_back(node);
            map.emplace(node->getName(), node);
            node->incrementDependencies();
        }
    }

    void incrementDependencies()
    {
        ++dependencies;
    }

    void decrementDependencies()
    {
        --dependencies;
    }

    int getNumberOfDependencies() const
    {
        return dependencies;
    }

    char getName() const
    {
        return name;
    }

    std::vector<std::shared_ptr<Project>>& getChidren()
    {
        return children;
    }
};

class Graph
{
    std::vector<std::shared_ptr<Project>> nodes;
    std::unordered_map<char, std::shared_ptr<Project>> map;
public:
    auto getOrCreateNode(char name)
    {
        if(map.find(name) == map.end())
        {
            auto node = std::make_shared<Project>(name);
            nodes.emplace_back(node);
            map.emplace(name, node);
        }

        return map[name];
    }

    void addEdge (char startName, char endName)
    {
        auto start = getOrCreateNode(startName);
        auto end = getOrCreateNode(endName);
        start->addNeighbor(end);
    }

    auto& getNodes()
    {
        return nodes;
    } 
};

Graph buildGraph(std::vector<char> &projects, std::vector<std::pair<char,char>> &dependencies)
{
    Graph graph;
    for(auto &project : projects)
    {
        graph.getOrCreateNode(project);
    }

    for(auto &depend : dependencies)
    {
        graph.addEdge(depend.first, depend.second);
    }

    return graph;
}

int addNonDependent(std::vector<Project*> &order, std::vector<std::shared_ptr<Project>> &projects, int offset)
{
    for(auto &project : projects)
    {
        if(project->getNumberOfDependencies() == 0)
        {
            order[offset] = project.get();
            ++offset;
        }
    }

    return offset;
}

auto orderProjects(std::vector<std::shared_ptr<Project>> &projects)
{
    std::vector<Project *> orders;
    orders.resize(projects.size());

    auto endOfList = addNonDependent(orders, projects, 0);
    auto toBeProcessed = 0;

    while(toBeProcessed < orders.size())
    {
        auto current = orders[toBeProcessed];
        if(current == nullptr)
        {
            return std::vector<Project*> ();
        }
        auto children = current->getChidren();
        for(auto &child : children)
        {
            child->decrementDependencies();
        }

        endOfList = addNonDependent(orders, children, endOfList);
        ++toBeProcessed;
    }


    return orders;
}

auto findBuildOrder(std::vector<char> &projects, std::vector<std::pair<char,char>> &dependencies)
{
   auto graph = buildGraph(projects, dependencies);
   return orderProjects(graph.getNodes());
}

struct testData
{
    std::vector<char> &projects;
    std::vector<std::pair<char,char>> &dependencies;
    std::string expected;
};

void test(const std::vector<testData> &testCases)
{
    for(const auto &testCase : testCases)
    {
        std::string result; 
        for(const auto &elem : findBuildOrder(testCase.projects, testCase.dependencies))
        {
            result+=elem->getName();
        }
        if(testCase.expected != result)
        {
            std::cout<<"Error. expected: "<<testCase.expected<<" given: "<<result<<'\n';
        }
    }
}

int main()
{
    std::vector<char> projects1{'a','b','c','d','e','f'};
    std::vector<std::pair<char,char>> dependencies1{{'a','d'},{'f','b'},{'b','d'}, {'f','a'}, {'d','c'}};

    std::vector<char> projects2{'z','q','n','m','v','w','p'};
    std::vector<std::pair<char,char>> dependencies2{{'q','z'},{'z','m'},{'v','q'}, {'m','p'}, {'n','w'}};
    
    std::vector<testData> testData
    {
        {projects1, dependencies1, "efbadc"},
        {projects2, dependencies2, "nvwqzmp"}
    };

    test(testData);


    return 0;
}