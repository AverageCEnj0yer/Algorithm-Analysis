#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <random>
#include <algorithm>



namespace DFS {

void traverse(const std::vector<std::vector<int>>& graph, int start) {
    std::stack<int> st;
    std::vector<bool> visited(graph.size(), false);

    st.push(start);

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (visited[u]) continue;

        visited[u] = true;
        std::cout << u << " ";

        for (int v : graph[u]) {
            st.push(v); // duplicates possible
        }
    }
}

}
namespace BFS {

void traverse(const std::vector<std::vector<int>>& graph, int start) {
    std::queue<int> q;
    std::vector<bool> visited(graph.size(), false);

    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (visited[u]) continue;

        visited[u] = true;
        std::cout << u << " ";

        for (int v : graph[u]) {
            q.push(v); // duplicates possible
        }
    }
}

}

namespace BFSo {

void traverse(const std::vector<std::vector<int>>& graph, int start) {
    std::queue<int> q;
    std::vector<bool> visited(graph.size(), false);

    visited[start] = true;              // OPT 1: mark when discovered
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        std::cout << u << " ";

        for (const int v : graph[u]) {  // OPT 4: const
            if (!visited[v]) {          // OPT 2: only push unvisited
                visited[v] = true;      // mark immediately
                q.push(v);
            }
        }
    }
}

}
namespace DFSo {

void traverse(const std::vector<std::vector<int>>& graph, int start) {
    std::stack<int> st;
    std::vector<bool> visited(graph.size(), false);

    visited[start] = true;              // OPT 1
    st.push(start);

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        std::cout << u << " ";

        for (const int v : graph[u]) {  // OPT 4
            if (!visited[v]) {          // OPT 2
                visited[v] = true;
                st.push(v);
            }
        }
    }
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
            for (int i = 0; i < numChildren && parent + i + 1 < n; ++i) {
                int child = parent + i + 1;
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
    std::vector<std::vector<int>> RandomGraph(int n, std::mt19937& rng, double p = 0.2) {
        std::vector<std::vector<int>> graph(n);
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) { // only u<v for undirected
                if (dist(rng) < p) {
                    graph[u].push_back(v);
                    graph[v].push_back(u);
                }
            }
        }
        return graph;
    }

}
int main()
{
    std::random_device rd {};
    std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    std::mt19937 mt{seed};

    return 0;
}
