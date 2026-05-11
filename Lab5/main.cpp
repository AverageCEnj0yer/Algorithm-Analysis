#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <random>
#include <string>
#include <utility>
#include <vector>

struct Edge {
	int u;
	int v;
	int w;
};

struct Graph {
	int n;
	std::vector<Edge> edges;
	std::vector<std::vector<std::pair<int, int>>> adj;

	Graph() : n(0) {}

	explicit Graph(int n_) : n(n_), adj(n_) {}

	void addEdge(int u, int v, int w) {
		edges.push_back({u, v, w});
		if (static_cast<int>(adj.size()) != n) {
			adj.assign(n, {});
		}
		adj[u].push_back({v, w});
		adj[v].push_back({u, w});
	}
};

struct SpanTree {
	Graph tree;
};

struct DSUBasic {
	std::vector<int> parent;

	explicit DSUBasic(int n) : parent(n) {
		std::iota(parent.begin(), parent.end(), 0);
	}

	int find(int x) {
		while (parent[x] != x) {
			x = parent[x];
		}
		return x;
	}

	bool unite(int a, int b) {
		int ra = find(a);
		int rb = find(b);
		if (ra == rb) {
			return false;
		}
		parent[rb] = ra;
		return true;
	}
};

struct DSUOptimized {
	std::vector<int> parent;
	std::vector<int> rank;

	explicit DSUOptimized(int n) : parent(n), rank(n, 0) {
		std::iota(parent.begin(), parent.end(), 0);
	}

	int find(int x) {
		if (parent[x] != x) {
			parent[x] = find(parent[x]);
		}
		return parent[x];
	}

	bool unite(int a, int b) {
		int ra = find(a);
		int rb = find(b);
		if (ra == rb) {
			return false;
		}
		if (rank[ra] < rank[rb]) {
			std::swap(ra, rb);
		}
		parent[rb] = ra;
		if (rank[ra] == rank[rb]) {
			rank[ra]++;
		}
		return true;
	}
};

static Graph generateConnectedGraph(int n, int extraEdges, std::mt19937 &rng) {
	Graph g(n);
	std::vector<std::vector<bool>> hasEdge(n, std::vector<bool>(n, false));
	std::uniform_int_distribution<int> weightDist(1, 1000);

	for (int i = 1; i < n; ++i) {
		std::uniform_int_distribution<int> parentDist(0, i - 1);
		int u = i;
		int v = parentDist(rng);
		int w = weightDist(rng);
		g.addEdge(u, v, w);
		hasEdge[u][v] = true;
		hasEdge[v][u] = true;
	}

	long long maxEdges = static_cast<long long>(n) * (n - 1) / 2;
	int maxExtra = static_cast<int>(std::min<long long>(extraEdges, maxEdges - (n - 1)));
	std::uniform_int_distribution<int> nodeDist(0, n - 1);
	int added = 0;
	while (added < maxExtra) {
		int u = nodeDist(rng);
		int v = nodeDist(rng);
		if (u == v || hasEdge[u][v]) {
			continue;
		}
		int w = weightDist(rng);
		g.addEdge(u, v, w);
		hasEdge[u][v] = true;
		hasEdge[v][u] = true;
		added++;
	}

	return g;
}

static SpanTree prim_basic(const Graph &g) {
	int n = g.n;
	const int INF = std::numeric_limits<int>::max() / 4;
	std::vector<std::vector<int>> mat(n, std::vector<int>(n, INF));
	for (const auto &e : g.edges) {
		if (e.w < mat[e.u][e.v]) {
			mat[e.u][e.v] = e.w;
			mat[e.v][e.u] = e.w;
		}
	}

	std::vector<int> key(n, INF);
	std::vector<int> parent(n, -1);
	std::vector<bool> inTree(n, false);
	key[0] = 0;

	for (int i = 0; i < n; ++i) {
		int u = -1;
		int best = INF;
		for (int v = 0; v < n; ++v) {
			if (!inTree[v] && key[v] < best) {
				best = key[v];
				u = v;
			}
		}
		if (u == -1) {
			break;
		}
		inTree[u] = true;
		for (int v = 0; v < n; ++v) {
			if (!inTree[v] && mat[u][v] < key[v]) {
				key[v] = mat[u][v];
				parent[v] = u;
			}
		}
	}

	Graph tree(n);
	for (int v = 1; v < n; ++v) {
		if (parent[v] == -1) {
			continue;
		}
		int w = mat[v][parent[v]];
		tree.addEdge(v, parent[v], w);
	}

	return {tree};
}

static SpanTree prim_optimized(const Graph &g) {
	int n = g.n;
	const int INF = std::numeric_limits<int>::max() / 4;
	std::vector<int> key(n, INF);
	std::vector<int> parent(n, -1);
	std::vector<bool> inTree(n, false);
	using Item = std::pair<int, int>;
	std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

	key[0] = 0;
	pq.push({0, 0});

	while (!pq.empty()) {
		auto [w, u] = pq.top();
		pq.pop();
		if (inTree[u]) {
			continue;
		}
		inTree[u] = true;
		for (const auto &nbr : g.adj[u]) {
			int v = nbr.first;
			int w2 = nbr.second;
			if (!inTree[v] && w2 < key[v]) {
				key[v] = w2;
				parent[v] = u;
				pq.push({key[v], v});
			}
		}
	}

	Graph tree(n);
	for (int v = 1; v < n; ++v) {
		if (parent[v] == -1) {
			continue;
		}
		int w = key[v];
		tree.addEdge(v, parent[v], w);
	}

	return {tree};
}

static SpanTree kruskal_basic(const Graph &g) {
	std::vector<Edge> edges = g.edges;
	std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
		return a.w < b.w;
	});

	DSUBasic dsu(g.n);
	Graph tree(g.n);

	for (const auto &e : edges) {
		if (dsu.unite(e.u, e.v)) {
			tree.addEdge(e.u, e.v, e.w);
			if (static_cast<int>(tree.edges.size()) == g.n - 1) {
				break;
			}
		}
	}

	return {tree};
}

static SpanTree kruskal_optimized(const Graph &g) {
	std::vector<Edge> edges = g.edges;
	std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
		return a.w < b.w;
	});

	DSUOptimized dsu(g.n);
	Graph tree(g.n);

	for (const auto &e : edges) {
		if (dsu.unite(e.u, e.v)) {
			tree.addEdge(e.u, e.v, e.w);
			if (static_cast<int>(tree.edges.size()) == g.n - 1) {
				break;
			}
		}
	}

	return {tree};
}

int main() {
	std::vector<int> sizes = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000};
	const int extraEdgesPerNode = 6;
	const int runs = 5;
	std::mt19937 rng(123456);

	std::ofstream unopt("unoptimised.csv");
	std::ofstream opt("optimised.csv");
	if (!unopt || !opt) {
		std::cerr << "Failed to open output CSV files.\n";
		return 1;
	}

	unopt << "vertices,edges,prim_ms,kruskal_ms\n";
	opt << "vertices,edges,prim_ms,kruskal_ms\n";

	static volatile std::size_t sink = 0;
	auto average_ms = [&](auto &&func) {
		double totalMs = 0.0;
		for (int i = 0; i < runs; ++i) {
			auto start = std::chrono::steady_clock::now();
			SpanTree tree = func();
			auto end = std::chrono::steady_clock::now();
			totalMs += std::chrono::duration<double, std::milli>(end - start).count();
			sink += tree.tree.edges.size();
		}
		return totalMs / runs;
	};

	struct Result {
		int n;
		std::size_t edges;
		double primMs;
		double kruskalMs;
	};

	std::vector<Result> unoptResults;
	std::vector<Result> optResults;

	for (int n : sizes) {
		int extraEdges = n * extraEdgesPerNode;
		Graph g = generateConnectedGraph(n, extraEdges, rng);

		double primBasicMs = average_ms([&]() { return prim_basic(g); });
		double kruskalBasicMs = average_ms([&]() { return kruskal_basic(g); });
		double primOptMs = average_ms([&]() { return prim_optimized(g); });
		double kruskalOptMs = average_ms([&]() { return kruskal_optimized(g); });

		unopt << n << "," << g.edges.size() << "," << std::fixed << std::setprecision(3)
			  << primBasicMs << "," << kruskalBasicMs << "\n";

		opt << n << "," << g.edges.size() << "," << std::fixed << std::setprecision(3)
			<< primOptMs << "," << kruskalOptMs << "\n";

		unoptResults.push_back({n, g.edges.size(), primBasicMs, kruskalBasicMs});
		optResults.push_back({n, g.edges.size(), primOptMs, kruskalOptMs});
	}

	auto print_table = [](const std::string &title, const std::vector<Result> &rows) {
		std::cout << "\n" << title << "\n";
		std::cout << std::left << std::setw(10) << "Vertices" << std::setw(10) << "Edges"
				  << std::setw(12) << "Prim(ms)" << std::setw(12) << "Kruskal(ms)" << "\n";
		std::cout << std::string(44, '-') << "\n";
		for (const auto &row : rows) {
			std::cout << std::left << std::setw(10) << row.n << std::setw(10) << row.edges
					  << std::setw(12) << std::fixed << std::setprecision(3) << row.primMs
					  << std::setw(12) << row.kruskalMs << "\n";
		}
	};

	print_table("Unoptimised (basic) results", unoptResults);
	print_table("Optimised results", optResults);

	std::cout << "Wrote unoptimised.csv and optimised.csv\n";
	return 0;
}
