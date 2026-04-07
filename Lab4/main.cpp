#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <cassert>

static constexpr int INF{std::numeric_limits<int>::max()};

using Edge = std::pair<int, int>;
using Graph = std::vector<std::vector<Edge>>;
using Matrix = std::vector<std::vector<int>>;

struct DijkstraResult {
	std::vector<int> distance;
	std::vector<int> predecessor;
};

struct FloydWarshallResult {
	Matrix distance;
	Matrix predecessor;
};

struct DistanceNodeCompare {
	bool operator()(const std::pair<int, int>& left, const std::pair<int, int>& right) const {
		return left.first > right.first;
	}
};

Graph generateGraph(int nodes, double p, int minWeight = 1, int maxWeight = 20) {
	if (nodes <= 0) {
		return {};
	}

	p = std::clamp(p, 0.0, 1.0);

	Graph graph(static_cast<std::size_t>(nodes));

	std::random_device randomDevice{};
	std::array<int, 8> seedData{};
	for (std::size_t i{0}; i < seedData.size(); ++i) {
		seedData[i] = randomDevice();
	}

	std::seed_seq seedSequence(seedData.begin(), seedData.end());
	std::mt19937 generator{seedSequence};

	std::uniform_int_distribution<int> warmupDistribution(
		0,
		std::numeric_limits<int>::max()
	);
	int warmupCount{10000};
	for (int i{0}; i < warmupCount; ++i) {
		static_cast<void>(warmupDistribution(generator));
	}

	std::uniform_real_distribution<double> nodeProbabilityDistribution(0.0, p);
	std::uniform_real_distribution<double> edgeDecisionDistribution(0.0, 1.0);
	std::uniform_int_distribution<int> weightDistribution(minWeight, maxWeight);

	for (int i{0}; i < nodes; ++i) {
		double nodeProbability{nodeProbabilityDistribution(generator)};
		for (int j{0}; j < nodes; ++j) {
			if (i == j) {
				continue;
			}

			double edgeRoll{edgeDecisionDistribution(generator)};
			if (edgeRoll <= nodeProbability) {
				int weight{weightDistribution(generator)};
				std::size_t sourceIndex{static_cast<std::size_t>(i)};
				graph[sourceIndex].push_back({j, weight});
			}
		}
	}

	return graph;
}

DijkstraResult dijkstra(const Graph& graph, int source) {
	int n{static_cast<int>(graph.size())};
	if (n == 0) {
		return {};
	}

	if (source < 0) {
		return {};
	}

	if (source >= n) {
		return {};
	}

    assert(source >= 0 && "Source index is less than 0");
	std::vector<int> distance(static_cast<std::size_t>(n), INF);
	std::vector<int> predecessor(static_cast<std::size_t>(n), -1);
	distance[source] = 0;

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, DistanceNodeCompare> queue{};
	queue.push({0, source});

	while (!queue.empty()) {
		std::pair<int, int> topEntry{queue.top()};
		queue.pop();

		int currentDistance{topEntry.first};
		int currentNode{topEntry.second};
		std::size_t currentIndex{static_cast<std::size_t>(currentNode)};
		if (currentDistance != distance[currentIndex]) {
			continue;
		}

		for (const std::pair<int, int>& edge : graph[currentIndex]) {
			int neighbor{edge.first};
			int weight{edge.second};
			std::size_t neighborIndex{static_cast<std::size_t>(neighbor)};

			if (distance[currentIndex] >= INF) {
				continue;
			}

            //this will never happen here but its something that can easily cause bugs - prevents overflow
			if (weight > INF - distance[currentIndex]) {
				continue;
			}

			int candidateDistance{distance[currentIndex] + weight};
			if (candidateDistance < distance[neighborIndex]) {
				distance[neighborIndex] = candidateDistance;
				predecessor[neighborIndex] = currentNode;
				queue.push({candidateDistance, neighbor});
			}
		}
	}

	return {distance, predecessor};
}

FloydWarshallResult floydWarshall(const Graph& graph) {
	int n{static_cast<int>(graph.size())};
	Matrix distance(
		static_cast<std::size_t>(n),
		std::vector<int>(static_cast<std::size_t>(n), INF)
	);
	Matrix predecessor(
		static_cast<std::size_t>(n),
		std::vector<int>(static_cast<std::size_t>(n), -1)
	);

	for (int i{0}; i < n; ++i) {
		std::size_t sourceIndex{static_cast<std::size_t>(i)};
		distance[sourceIndex][sourceIndex] = 0;
		predecessor[sourceIndex][sourceIndex] = i;

		for (const std::pair<int, int>& edge : graph[sourceIndex]) {
			int neighbor{edge.first};
			int weight{edge.second};
			std::size_t neighborIndex{static_cast<std::size_t>(neighbor)};
			if (weight < distance[sourceIndex][neighborIndex]) {
				distance[sourceIndex][neighborIndex] = weight;
				predecessor[sourceIndex][neighborIndex] = i;
			}
		}
	}
	//====at this point ditance matrix and precedence matrix have been built====
	for (int k{0}; k < n; ++k) {
		for (int i{0}; i < n; ++i) {
			std::size_t iIndex{static_cast<std::size_t>(i)};
			std::size_t kIndex{static_cast<std::size_t>(k)};
			if (distance[iIndex][kIndex] >= INF) {
				continue;
			}

			for (int j{0}; j < n; ++j) {
				std::size_t jIndex{static_cast<std::size_t>(j)};
				if (distance[kIndex][jIndex] >= INF) {
					continue;
				}

				if (distance[iIndex][kIndex] > INF - distance[kIndex][jIndex]) {
					continue;
				}

				int candidateDistance{distance[iIndex][kIndex] + distance[kIndex][jIndex]};
				if (candidateDistance < distance[iIndex][jIndex]) {
					distance[iIndex][jIndex] = candidateDistance;
					predecessor[iIndex][jIndex] = predecessor[kIndex][jIndex];
				}
			}
		}
	}

	return {distance, predecessor};
}
//------------------------------------------------------------------
//--------------------start of for debugging------------------------
//------------------------------------------------------------------
std::vector<int> buildPath( int source, int target,
							const std::vector<int>& predecessor,
							const std::vector<int>& distance) {
	if (source < 0 || target < 0) {
		return {};
	}

	if (distance.empty()) {
		return {};
	}

	std::size_t sourceIndex{static_cast<std::size_t>(source)};
	std::size_t targetIndex{static_cast<std::size_t>(target)};
	if (sourceIndex >= distance.size() || targetIndex >= distance.size()) {
		return {};
	}

	if (distance[targetIndex] >= INF) {
		return {};
	}

	std::vector<int> path{};
	int current{target};
	while (current != source) {
		path.push_back(current);
		int previous{predecessor[static_cast<std::size_t>(current)]};
		if (previous < 0) {
			return {};
		}
		current = previous;
	}

	path.push_back(source);
	std::reverse(path.begin(), path.end());
	return path;
}
void printGraph(const Graph& graph) {
	int n{static_cast<int>(graph.size())};
	for (int i{0}; i < n; ++i) {
		std::size_t sourceIndex{static_cast<std::size_t>(i)};
		std::cout << "Node " << i << ": ";
		if (graph[sourceIndex].empty()) {
			std::cout << "(no outgoing edges)";
		}

		for (const std::pair<int, int>& edge : graph[sourceIndex]) {
			int neighbor{edge.first};
			int weight{edge.second};
			std::cout << "(" << neighbor << ", w=" << weight << ") ";
		}
		std::cout << '\n';
	}
}
void printMatrix(const Matrix& matrix) {
	int n{static_cast<int>(matrix.size())};
	for (int i{0}; i < n; ++i) {
		for (int j{0}; j < n; ++j) {
			std::size_t iIndex{static_cast<std::size_t>(i)};
			std::size_t jIndex{static_cast<std::size_t>(j)};
			if (matrix[iIndex][jIndex] >= INF) {
				std::cout << std::setw(5) << "INF";
			} else {
				std::cout << std::setw(5) << matrix[iIndex][jIndex];
			}
		}
		std::cout << '\n';
	}
}
void printDistancesFromSource(const std::vector<int>& distance, int source) {
	std::cout << "Distances from source node " << source << ":\n";
	for (std::size_t i{0}; i < distance.size(); ++i) {
		std::cout << "  to " << i << " = ";
		if (distance[i] >= INF) {
			std::cout << "INF";
		} else {
			std::cout << distance[i];
		}
		std::cout << '\n';
	}
}
void printPredecessorMatrix(const Matrix& predecessor) {
	int n{static_cast<int>(predecessor.size())};
	for (int i{0}; i < n; ++i) {
		for (int j{0}; j < n; ++j) {
			std::size_t iIndex{static_cast<std::size_t>(i)};
			std::size_t jIndex{static_cast<std::size_t>(j)};
			std::cout << std::setw(5) << predecessor[iIndex][jIndex];
		}
		std::cout << '\n';
	}
}
//----------------------------------------------------------------
//--------------------end of for debugging------------------------
//----------------------------------------------------------------
int main() {
	const std::vector<int> floydNodeCounts{
		100,
		200,
		300,
		400,
		500,
		600,
		700
	};
	const std::vector<int> dijkstraNodeCounts{
		500,
		1000,
		1500,
		2000,
		2500,
		3000,
		3500
	};
	struct GraphConfig {
		const char* label;
		double p;
	};
	const std::vector<GraphConfig> configs{
		{"dense", 0.8},
		{"semi_dense", 0.5},
		{"sparse", 0.15}
	};
	const int runs{5};
	const int source{0};

	std::cout << std::fixed << std::setprecision(3);

	for (const GraphConfig& config : configs) {
		std::string floydFileName{"Floyd_"};
		floydFileName += config.label;
		floydFileName += ".csv";
		std::string dijkstraFileName{"dijkstra_"};
		dijkstraFileName += config.label;
		dijkstraFileName += ".csv";

		std::ofstream floydFile(floydFileName);
		std::ofstream dijkstraFile(dijkstraFileName);
		if (!dijkstraFile || !floydFile) {
			std::cerr << "Failed to open output files for " << config.label << ".\n";
			return 1;
		}

		floydFile << "nodes,avg_ms\n";
		dijkstraFile << "nodes,avg_ms\n";

		std::cout << "\nGraph type: " << config.label << " (p=" << config.p << ")\n";
		std::cout << "Floyd-Warshall timings (avg of " << runs << ")\n";
		std::cout << std::left << std::setw(8) << "Nodes"
			<< std::setw(16) << "Floyd(ms)" << '\n';

		for (int nodes : floydNodeCounts) {
			Graph graph{generateGraph(nodes, config.p)};
			if (graph.empty()) {
				std::cout << std::left << std::setw(8) << nodes
					<< std::setw(16) << "N/A" << '\n';
				continue;
			}

			double floydTotal{0.0};
			for (int run{0}; run < runs; ++run) {
				auto start{std::chrono::steady_clock::now()};
				FloydWarshallResult result{floydWarshall(graph)};
				auto end{std::chrono::steady_clock::now()};
				std::chrono::duration<double, std::milli> elapsed{end - start};
				floydTotal += elapsed.count();
				if (result.distance.empty()) {
					std::cerr << "Floyd-Warshall failed for nodes=" << nodes << ".\n";
					return 1;
				}
			}
			double floydAvg{floydTotal / static_cast<double>(runs)};

			floydFile << nodes << ',' << floydAvg << '\n';

			std::cout << std::left << std::setw(8) << nodes
				<< std::setw(16) << floydAvg << '\n';
		}

		std::cout << "\nDijkstra timings (avg of " << runs << ")\n";
		std::cout << std::left << std::setw(8) << "Nodes"
			<< std::setw(16) << "Dijkstra(ms)" << '\n';

		for (int nodes : dijkstraNodeCounts) {
			Graph graph{generateGraph(nodes, config.p)};
			if (graph.empty()) {
				std::cout << std::left << std::setw(8) << nodes
					<< std::setw(16) << "N/A" << '\n';
				continue;
			}

			double dijkstraTotal{0.0};
			for (int run{0}; run < runs; ++run) {
				auto start{std::chrono::steady_clock::now()};
				DijkstraResult result{dijkstra(graph, source)};
				auto end{std::chrono::steady_clock::now()};
				std::chrono::duration<double, std::milli> elapsed{end - start};
				dijkstraTotal += elapsed.count();
				if (result.distance.empty()) {
					std::cerr << "Dijkstra failed for nodes=" << nodes << ".\n";
					return 1;
				}
			}
			double dijkstraAvg{dijkstraTotal / static_cast<double>(runs)};

			dijkstraFile << nodes << ',' << dijkstraAvg << '\n';

			std::cout << std::left << std::setw(8) << nodes
				<< std::setw(16) << dijkstraAvg << '\n';
		}
	}

	return 0;
}
