#ifndef READWRITER_H_
#define READWRITER_H_

#include "testlib.h"
#include "bits/stdc++.h"
#include <filesystem>

enum PrintFormat {
    Prompt,
    Solution
};


// map for keeping the names of directories
// promptInputDirectory - directory for in files formatted for prompt
// promptInputDirectory - directory for in files formatted for the solution
const std::map<std::string, std::string> dirs = {
    {"promptInputDirectory", "in"},
    {"solutionInputDirectory", "solution-in"}
};

template <typename F, typename S>
std::ostream &operator<<(std::ostream &os, const std::pair<F, S> &p)
{
    return os << "(" << p.first << ", " << p.second << ")";
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    os << "{";
    typename std::vector<T>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        if (it != v.begin()) {
            os << ", ";
        }
        os << *it;
    }
    return os << "}";
}

void println() { std::cout << '\n'; }
template <typename Head, typename... Tail>
void println(Head H, Tail... T)
{
    std::cout << ' ' << H;
    println(T...);
}

class Graph {
public:
    bool directed = false;
    int numberOfNodes;
    std::vector<std::vector<int>> graph;
    int numberOfEdges;

private:
    void printForPromptTo(std::ostream &outputStream) const {
        outputStream << "{";
        for (int i = 0; i < numberOfNodes; ++i) {
            outputStream << "{";
            for (int j = 0; j < graph[i].size(); ++j) {
                outputStream << graph[i][j];
                if (j != graph[i].size() - 1) {
                    outputStream << ",";
                }
            }
            outputStream << "}";
            if (i != numberOfNodes - 1) {
                outputStream << ",";
            }
        }
        outputStream <<  "}\n";
    }

    void printForSolutionTo(std::ostream &outputStream) const {
        auto edges = getEdges();

        outputStream << numberOfNodes << " " << edges.size() << "\n";
        for(auto edge: edges) {
            outputStream << edge.first << " " << edge.second << "\n";
        }
    }

public:
    Graph(int nodes, std::vector<std::vector<int>> g, bool directed = false)
        : directed(directed),
        numberOfNodes(nodes),
        graph(g),
        numberOfEdges(
            std::accumulate(graph.begin(), 
                            graph.end(), 
                            0, 
                            [](const int x, const auto &a) {
                                return x + a.size(); 
                            }
            )
        ) {}

    bool operator==(const Graph& other) const {
        if (numberOfNodes != other.numberOfNodes) {
            return false;
        }

        for(int node = 0; node < numberOfNodes ; ++node) {
            if(graph[node] != other.graph[node]) {
                return false;
            }
        }
        return true;        
    }

    Graph& relabelNodes() {
        auto perm = rnd.perm(numberOfNodes);
        std::vector<int> inv_perm(numberOfNodes);
        for(int i = 0; i < numberOfNodes; i++) {
            inv_perm[perm[i]] = i;
        }
        auto graph_copy = graph;
        for(int v = 0; v < numberOfNodes; ++v) {
            graph[v] = graph_copy[inv_perm[v]];
            for(auto &neigh : graph[v]) {
                neigh = perm[neigh];
            }
        }
        return *this;
    }

    std::vector<std::pair<int,int>> getEdges() const {
        std::vector<std::pair<int,int>> edges;
        for(int v = 0; v < numberOfNodes; ++v)
            for(int u : graph[v])
                edges.emplace_back(v, u);
        return edges;
    }
    
    void printTo(std::ostream &outputStream, PrintFormat format) const {
        switch (format) {
            case Prompt:
                printForPromptTo(outputStream);
                break;
            case Solution:
                printForSolutionTo(outputStream);
                break;
        }
    }

    static Graph construct_empty_graph(int nodes) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        return Graph(nodes, g);
    }

    static Graph construct_undirected_clique(int nodes) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        for (int i = 0; i < nodes; ++i) {
            for (int j = 0; j < nodes; ++j) {
                if (i == j) {
                    continue;
                }
                
                g[i].push_back(j);
            }
        } 
        
        return Graph(nodes, g);
    }

    static Graph construct_path_graph(int nodes, int numberOfComponents = 1) {
        std::vector<std::vector<int>> g(nodes);
        std::vector<int> part = rnd.partition(numberOfComponents, nodes);
        int current = 0;
        for(int l = 0; l < part.size(); ++l) {
            int length = part[l];
            for(int i = 0 ; i < length - 1; ++i) {
                g[current].push_back(current + 1);
                g[current + 1].push_back(current);
                ++current;
            }
            ++current;
        }
        return Graph(nodes, g).relabelNodes();
    }

    static Graph construct_shallow_forest_graph(int nodes, int numberOfTrees) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        std::vector<int> pa = rnd.partition(numberOfTrees, nodes);
        int root = 0, sum = 0, pnt = 0;
        for(int i = 0; i < nodes; i++) {
            if(i == sum) {
                root = sum;
                sum += pa[pnt++];
                continue;
            }
            int neighbor = rnd.next(root, i);
            g[neighbor].push_back(i);
            g[i].push_back(neighbor);
        }
        return Graph(nodes, g).relabelNodes();
    }

    static Graph construct_shallow_tree_graph(int nodes) {
        return construct_shallow_forest_graph(nodes, 1);
    }
    
    static Graph construct_forest_graph(int nodes, int numberOfTrees) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        std::vector<int> pa = rnd.partition(numberOfTrees, nodes);
        int root = 0;
        for(auto currentNodes : pa) {
            std::vector<int> prufer, cnt(currentNodes, 0);
            std::vector<std::vector<int>> g_curr(currentNodes);
            if(currentNodes == 1) {
                root++;
                continue;
            } 
            if(currentNodes == 2) {
                g[root].push_back(root + 1);
                g[root + 1].push_back(root);
                root += 2;
                continue;
            }
            for (int i = 0; i < currentNodes - 2; i++) {
                int x = rnd.next(currentNodes);
                prufer.push_back(x);
                ++cnt[x];
            }
            std::priority_queue<int> q;
            for (int i = 0; i < currentNodes; ++i) {
                if (!cnt[i]) {
                    q.push(i);
                }
            }
            for (auto v : prufer) {
                int u = q.top();
                g_curr[u].push_back(v);
                g_curr[v].push_back(u);
                q.pop();
                if (--cnt[v] == 0) {
                    q.push(v);
                }
            }
            int x = q.top(); q.pop();
            int y = q.top();
            g_curr[x].push_back(y);
            g_curr[y].push_back(x);

            std::queue<int> bfs;

            bfs.push(0);
            int _id = root;
            while (!bfs.empty())
            {
                int u = bfs.front();
                cnt[u] = 1;
                bfs.pop();
                for (auto v : g_curr[u])
                {
                    if (cnt[v] == 0) {
                        root++;
                        g[_id].push_back(root);
                        g[root].push_back(_id);
                        bfs.push(v);
                    }
                }
                ++_id;
            }
        }
        return Graph(nodes, g).relabelNodes();
    }

    static Graph construct_tree_graph(int nodes) {
        return construct_forest_graph(nodes, 1);
    }

    static Graph construct_simpler_jellyfish_graph(int nodes, int cycleSize, int maxTentacleLength, int numberOfTentacles) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        std::vector<int> pa = rnd.partition(nodes - cycleSize, numberOfTentacles, maxTentacleLength);
        int next = 1;
        int prev = 0;
        for(int i = 0; i < cycleSize - 1; i++) {
            g[prev].push_back(next);
            g[next].push_back(prev);
            prev = next;
            next++;
        }
        g[prev].push_back(0);
        g[0].push_back(prev);
        for(int raySize : pa) {
            prev = rnd.next(cycleSize); // Ray starts at node in [0, cycyleSize - 1]
            for(int i = 0; i < raySize; i++) {
                g[prev].push_back(next);
                g[next].push_back(prev);
                prev = next;
                next++;
            }
        }
        return Graph(nodes, g).relabelNodes();
    }

    static Graph construct_starfish_graph(int nodes, int maxRayLength, int numberOfRays) {
        return construct_simpler_jellyfish_graph(nodes, 1, maxRayLength, numberOfRays);
    }

    /* Silkworm of size n is a path of size (n+1)/2 and one private node for each node from path */
    static Graph construct_silkworm_graph(int nodes) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        for (int i = 0; i < nodes; i += 2) {
            if (i+1 < nodes) {
                g[i].push_back(i+1);
                g[i+1].push_back(i);
            }
            if (i+2 < nodes) {
                g[i].push_back(i+2);
                g[i+2].push_back(i);
            }
        }
        
        return Graph(nodes, g).relabelNodes();
    }
    

    static Graph construct_tree_of_bounded_degree_graph(int nodes, int minDegree, int maxDegree) {
        std::vector<std::vector<int>> g(nodes);
        std::vector<int> vec(nodes); std::iota(begin(vec), end(vec), 0);
        std::deque<int> availableLeaves(std::begin(vec), std::end(vec));
        std::queue<int> inTree; 
        availableLeaves.pop_front(); inTree.push(0); // move 0 from availableLeaves to inTree
        while(!availableLeaves.empty() && !inTree.empty()) {
            int currentNode = inTree.front(); inTree.pop();
            int degree = rnd.next(std::min(minDegree, (int)availableLeaves.size()), std::min(maxDegree, (int)availableLeaves.size()));
            while(degree--) {
                int nextNode = availableLeaves.front(); availableLeaves.pop_front();
                inTree.push(nextNode);
                g[currentNode].push_back(nextNode);
            }
        }
        return Graph(nodes, g).relabelNodes();
    }
    
    // fixme: placeholder
    static Graph construct_sparse_graph(int nodes) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        return Graph(nodes, g);
    }

    // fixme: placeholder
    static Graph construct_dense_graph(int nodes) {
        std::vector<std::vector<int>> g;
        g.resize(nodes);
        return Graph(nodes, g);
    }
    
    // Ścieżka // Zbiór ścieżek
    // Drzewo // Las
    // Losowe gęste // Losowe rzadkie
    // Gwiazda // Rozgwiazda
    // Silkworm
    // Meduza
    // Drzewa n-arne
    // Graf r-regularny
    // Dwudzielne
    // Bi-clique
    // Krata (n x m)
    // Grid (n x m)

    bool is_clique() {
        return (directed ? numberOfEdges : 2*numberOfEdges) == numberOfNodes*(numberOfNodes-1);
    }
    
    bool is_connected() {
        return undirected_connected_components_number() == 1;
    }

    int undirected_connected_components_number() {
        int scc_number = 0;
        bool visited[numberOfNodes];
        
        std::function<void (int)> dfs = [&](int v) -> void {
            visited[v] = true;
            for (auto u : graph[v]){
                dfs(u);
            }    
        };
        
        for(int v = 0; v < numberOfNodes; v++) {
            if (!visited[v]) {
                visited[v] = true;
                scc_number++;
                dfs(v);
            }
        }
        return scc_number;
    }    
};

void setupDirectories() {
    for (const auto& dir : dirs) {
        if(!std::filesystem::create_directory(dir.second)) {
            // Only warning, because you can fail to create a directory if it exists.
            std::cerr << "Warning: Could not create directory " << dir << std::endl;
        }
    }
}

/** 
 * @brief Returns 2 streams: for prompt input files and for solution input files.
 * 
 * @note The caller is responsible for closing the file streams using its close() method.
 */
std::pair<std::ofstream, std::ofstream> setupTest(int testNumber) {
    std::string promptInPath = format("%s/%d.in", dirs.at("promptInputDirectory").c_str(), testNumber);
    std::string solutionInPath = format("%s/%d.in", dirs.at("solutionInputDirectory").c_str(), testNumber);

    std::ofstream promptInFile(promptInPath);
    if (!promptInFile) {
        std::cerr << "Error: Could not open the file " << promptInPath << std::endl;
        exit(1);
    }
    std::ofstream solutionInFile(solutionInPath);
    if (!solutionInFile) {
        std::cerr << "Error: Could not open the file " << solutionInPath << std::endl;
        exit(1);
    }
    return {std::move(promptInFile), std::move(solutionInFile)};
}

#endif
