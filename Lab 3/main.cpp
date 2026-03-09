#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include <functional>
#include <chrono>

struct Metrics {
    size_t nodesVisited = 0;
    size_t maxMemory = 0;   // queue size / stack size / recursion depth
};

namespace DFS {


void dfs_internal(int u,
                  const std::vector<std::vector<int>>& graph,
                  std::vector<bool>& visited,
                  Metrics& m,
                  size_t currentStackSize)
{
    if (visited[u]) return;

    visited[u] = true;
    m.nodesVisited++;

    m.maxMemory = std::max(m.maxMemory, currentStackSize);

    for (int v : graph[u]) {
        dfs_internal(v, graph, visited, m, currentStackSize + 1);
    }
}

Metrics traverse(const std::vector<std::vector<int>>& graph, int start) {
    Metrics m;

    if (start < 0 || start >= graph.size()) return m;

    std::vector<bool> visited(graph.size(), false);

    dfs_internal(start, graph, visited, m, 1);

    return m;
}

bool find(const std::vector<std::vector<int>>& graph, int start, int target) {
    std::vector<bool> visited(graph.size(), false);

    std::function<bool(int)> dfs = [&](int u) {
        if (visited[u]) return false;
        visited[u] = true;

        if (u == target) return true;

        for (int v : graph[u])
            if (dfs(v)) return true;

        return false;
    };

    return dfs(start);
}

}
namespace DFSo {

void dfs_internal(int u,
                  const std::vector<std::vector<int>>& graph,
                  std::vector<char>& visited,
                  Metrics& m,
                  size_t currentStackSize)
{
    visited[u] = 1;            // OPT: early visited marking
    m.nodesVisited++;
    m.maxMemory = std::max(m.maxMemory, currentStackSize); // OPT: track max stack size

    const auto& neighbors = graph[u]; // OPT: adjacency list caching

    for (int v : neighbors) {
        if (!visited[v]) {              // OPT: pre-recursion visited check
            dfs_internal(v, graph, visited, m, currentStackSize + 1);
        }
    }
}

Metrics traverse(const std::vector<std::vector<int>>& graph, int start) {
    Metrics m;

    const int n = graph.size();      // OPT: cache graph size
    if (start < 0 || start >= n) return m;

    std::vector<char> visited(n, 0); // OPT: vector<char> instead of vector<bool>

    dfs_internal(start, graph, visited, m, 1);

    return m;
}

bool dfs_find_internal(int u,
                       int target,
                       const std::vector<std::vector<int>>& graph,
                       std::vector<char>& visited)
{
    visited[u] = 1;                  // OPT: early visited marking

    if (u == target) return true;

    for (int v : graph[u]) {
        if (!visited[v]) {           // OPT: pre-recursion visited check
            if (dfs_find_internal(v, target, graph, visited))
                return true;
        }
    }

    return false;
}

bool find(const std::vector<std::vector<int>>& graph, int start, int target) {
    const int n = graph.size();
    if (start < 0 || start >= n) return false;

    std::vector<char> visited(n, 0); // OPT: vector<char> instead of vector<bool>

    return dfs_find_internal(start, target, graph, visited);
}

}

namespace BFS {

Metrics traverse(const std::vector<std::vector<int>>& graph, int start) {
    Metrics m;

    std::queue<int> q;
    std::vector<bool> visited(graph.size(), false);

    q.push(start);

    while (!q.empty()) {

        m.maxMemory = std::max(m.maxMemory, q.size());

        int u = q.front();
        q.pop();

        if (visited[u]) continue;

        visited[u] = true;
        m.nodesVisited++;

        for (int v : graph[u]) {
            q.push(v);
        }
    }

    return m;
}

bool find(const std::vector<std::vector<int>>& graph, int start, int target) {
    std::queue<int> q;
    std::vector<bool> visited(graph.size(), false);

    q.push(start);

    while (!q.empty()) {

        int u = q.front();
        q.pop();

        if (visited[u]) continue;

        visited[u] = true;

        if (u == target) return true;

        for (int v : graph[u]) {
            q.push(v);
        }
    }

    return false;
}

}
namespace BFSo {

Metrics traverse(const std::vector<std::vector<int>>& graph, int start) {
    Metrics m;

    std::queue<int> q;
    std::vector<bool> visited(graph.size(), false);

    visited[start] = true; // OPT: early visited marking (discovery)
    q.push(start);

    while (!q.empty()) {

        m.maxMemory = std::max(m.maxMemory, q.size()); // OPT: track max memory

        int u = q.front();
        q.pop();

        m.nodesVisited++;

        for (const int v : graph[u]) { // OPT: const loop variable
            if (!visited[v]) {         // OPT: pre-enqueue visited check
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return m;
}

bool find(const std::vector<std::vector<int>>& graph, int start, int target) {
    std::queue<int> q;
    std::vector<bool> visited(graph.size(), false);

    if (start == target) return true;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        
        int u = q.front();
        q.pop();

        for (const int v : graph[u]) {
            if (!visited[v]) {
                if (v == target) return true;  // immediate discovery
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return false;
}


}

namespace GraphGeneration {

    // ----------------------------
    // 1. Tree generator (undirected)
    // Each node can have 2 or more children
    // ----------------------------
    std::vector<std::vector<int>> Tree(int n, std::mt19937& rng, int minChildren = 2, int maxChildren = 3) {
        std::vector<std::vector<int>> graph(n);
        for (int parent = 0; parent < n - 1; ++parent) {
            int numChildren = std::uniform_int_distribution<int>(minChildren, maxChildren)(rng);
            for (int i = 0; i < numChildren && parent*2 + i + 1 < n; ++i) {
                int child = parent*2 + i + 1;
                graph[parent].push_back(child);
                graph[child].push_back(parent); // undirected
            }
        }
        return graph;
    }

    // ----------------------------
    // 2. Complete graph generator (undirected)
    // ----------------------------
    std::vector<std::vector<int>> CompleteGraph(int n) {
        std::vector<std::vector<int>> graph(n);
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) { // only u<v to avoid duplicates
                graph[u].push_back(v);
                graph[v].push_back(u); // undirected
            }
        }
        return graph;
    }

    // ----------------------------
    // 3. Almost linear graph (undirected)
    // Linear backbone with occasional extra edges
    // ----------------------------
    std::vector<std::vector<int>> AlmostLinearGraph(int n, std::mt19937& rng, double extraEdgeProb = 0.1) {
        std::vector<std::vector<int>> graph(n);
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int i = 0; i < n - 1; ++i) {
            // backbone edge
            graph[i].push_back(i + 1);
            graph[i + 1].push_back(i);

            // occasional extra forward edges
            for (int j = i + 2; j < n; ++j) {
                if (dist(rng) < extraEdgeProb) {
                    graph[i].push_back(j);
                    graph[j].push_back(i); // undirected
                }
            }
        }
        return graph;
    }

    // ----------------------------
    // 4. Random graph generator (undirected)
    // Each possible edge exists with probability p
    // ----------------------------
    std::vector<std::vector<int>> RandomGraph(int n, std::mt19937& rng) {
        std::vector<std::vector<int>> graph(n);
        std::uniform_real_distribution<double> dist(0.0, 0.1); //THIS IS SO GOOD, INSTEAD OF HAVING DIST USED ON POINTER (SEE BELOW, WE CREATE A TEST!)
        std::uniform_real_distribution<double> test(0.0, 1.00);
        std::vector<double> node_probs(n);
        // Assign a random probability to each node
        for (int u = 0; u < n; ++u) {
            node_probs[u] = dist(rng); // Each node gets a random p in [0,1)
        }

        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) { // only u<v for undirected
                // Use the average of node u and v's probabilities for edge existence
                double edge_prob = (node_probs[u] + node_probs[v]) / 2.0;
                if (test(rng) < edge_prob) { // POININTER FOR PREVIOUS CAPS COMMENT. DIST(RNG)
                    graph[u].push_back(v);
                    graph[v].push_back(u);
                }
            }
        }
        return graph;
    }

}
// int main()
// {
//     std::random_device rd {};
//     std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
//     std::mt19937 mt{seed};

//     auto g1 = GraphGeneration::CompleteGraph(10000);

//     auto start = std::chrono::steady_clock::now();
//     BFSo::find(g1, 0, 9000);
//     auto end = std::chrono::steady_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//     std::cout << "Time taken: " << duration.count() << "\n";
//     // std::cout << "Max Memory: " << temp << "\n";
//     return 0;
// }
