#ifndef GRAPH_H_
#define GRAPH_H_

#include "utils.h"

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
        outputStream << "}\n";
    }

    void printForSolutionTo(std::ostream &outputStream) const {
        auto edges = getEdges();

        outputStream << numberOfNodes << " " << edges.size() << "\n";
        for (auto edge : edges) {
            outputStream << edge.first << " " << edge.second << "\n";
        }
    }

   public:
    Graph(std::vector<std::vector<int>> g, bool directed = false)
        : directed(directed),
          numberOfNodes(g.size()),
          graph(g),
          numberOfEdges(std::accumulate(graph.begin(), graph.end(), 0,
                                        [](const int x, const auto &a) { return x + a.size(); })) {}

    bool operator==(const Graph &other) const {
        if (numberOfNodes != other.numberOfNodes) {
            return false;
        }

        for (int node = 0; node < numberOfNodes; ++node) {
            if (graph[node] != other.graph[node]) {
                return false;
            }
        }
        return true;
    }

    operator std::vector<std::vector<int>>() const { return graph; }

    std::vector<std::pair<int, int>> getEdges() const {
        std::vector<std::pair<int, int>> edges;
        for (int v = 0; v < numberOfNodes; ++v)
            for (int u : graph[v]) edges.emplace_back(v, u);
        return edges;
    }

    Graph &relabelNodes();

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

    static Graph read_graph(std::istream &inputStream) {
        int nodes, numberOfEdges;
        inputStream >> nodes >> numberOfEdges;
        std::vector<std::vector<int>> g(nodes);
        for (int i = 0; i < numberOfEdges; i++) {
            int a, b;
            inputStream >> a >> b;
            g[a].push_back(b);
        }
        return Graph(g);
    }

    static Graph construct_empty_graph(int nodes);
    static Graph construct_undirected_clique(int nodes);
    static Graph construct_path_graph(int nodes, int numberOfComponents = 1);
    static Graph construct_shallow_forest_graph(int nodes, int numberOfTrees);
    static Graph construct_shallow_tree_graph(int nodes);
    static Graph construct_forest_graph(int nodes, int numberOfTrees);
    static Graph construct_tree_graph(int nodes);
    static Graph construct_simpler_jellyfish_graph(int nodes, int cycleSize, int maxTentacleLength,
                                                   int numberOfTentacles);
    static Graph construct_starfish_graph(int nodes, int maxRayLength, int numberOfRays);
    static Graph construct_silkworm_graph(int nodes);
    static Graph construct_tree_of_bounded_degree_graph(int nodes, int minDegree, int maxDegree);
    static Graph construct_sparse_graph(int nodes);
    static Graph construct_dense_graph(int nodes);

    bool is_clique() { return (directed ? numberOfEdges : 2 * numberOfEdges) == numberOfNodes * (numberOfNodes - 1); }

    bool is_connected() { return undirected_connected_components_number() == 1; }

    int undirected_connected_components_number() {
        int scc_number = 0;
        bool visited[numberOfNodes];

        std::function<void(int)> dfs = [&](int v) -> void {
            visited[v] = true;
            for (auto u : graph[v]) {
                dfs(u);
            }
        };

        for (int v = 0; v < numberOfNodes; v++) {
            if (!visited[v]) {
                visited[v] = true;
                scc_number++;
                dfs(v);
            }
        }
        return scc_number;
    }
};

#endif
