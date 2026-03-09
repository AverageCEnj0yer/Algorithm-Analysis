#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <functional>

#include "main.cpp"

// Assume Metrics, GraphGeneration, DFS, DFSo, BFS, BFSo are defined in main.cpp or included headers
// For this template, we will use function pointers and assume the same signatures as in your main.cpp

using Graph = std::vector<std::vector<int>>;
using TraverseFunc = std::function<Metrics(const Graph&, int)>;
using FindFunc = std::function<bool(const Graph&, int, int)>;

struct Algo {
    std::string name;
    TraverseFunc traverse;
    FindFunc find;
};

struct GraphGen {
    std::string name;
    std::function<Graph(int, std::mt19937&)> gen;
};

const std::vector<int> sizes = {500, 1000, 2000, 5000, 10000};
const int find_trials = 10;

void ensure_dir(const std::string& path) {
    std::filesystem::create_directories(path);
}

void write_csv(const std::string& path, const std::vector<int>& sizes, const std::vector<double>& times, const std::vector<size_t>& maxmem) {
    std::ofstream out(path);
    out << "Size,Time_us\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        out << sizes[i] << "," << times[i] << "\n";
    }
    out << "maxMemory";
    for (size_t m : maxmem) out << "," << m;
    out << "\n";
}

void write_find_csv(const std::string& path, const std::vector<int>& sizes, const std::vector<double>& times) {
    std::ofstream out(path);
    out << "Size,AvgFindTime_us\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        out << sizes[i] << "," << times[i] << "\n";
    }
}

int main() {
    using namespace std;
    using namespace std::chrono;

    // Seed RNG for reproducibility
    std::mt19937 rng(42);

    // Graph generators
    vector<GraphGen> graphGens = {
        {"Tree", [](int n, mt19937& rng) { return GraphGeneration::Tree(n, rng, 2, 2); }},
        {"CompleteGraph", [](int n, mt19937&) { return GraphGeneration::CompleteGraph(n); }},
        {"AlmostLinearGraph", [](int n, mt19937& rng) { return GraphGeneration::AlmostLinearGraph(n, rng); }},
        {"RandomGraph", [](int n, mt19937& rng) { return GraphGeneration::RandomGraph(n, rng); }}
    };

    // Algorithms
    vector<Algo> algos = {
        {"DFS", DFS::traverse, DFS::find},
        {"DFSo", DFSo::traverse, DFSo::find},
        {"BFS", BFS::traverse, BFS::find},
        {"BFSo", BFSo::traverse, BFSo::find}
    };

    string base = "/home/gabi/University Stuff/AA sem 4/Lab 3";

    for (const auto& ggen : graphGens) {
        cout << "\n==== " << ggen.name << " ====" << endl;
        for (const auto& algo : algos) {
            cout << "\n-- " << algo.name << " --" << endl;
            vector<double> traverse_times, find_times;
            vector<size_t> maxmems;
            for (int n : sizes) {
                // Generate graph
                Graph g = ggen.gen(n, rng);
                // Traverse benchmark
                auto t1 = high_resolution_clock::now();
                Metrics m = algo.traverse(g, 0);
                auto t2 = high_resolution_clock::now();
                double t_us = duration_cast<microseconds>(t2 - t1).count();
                traverse_times.push_back(t_us);
                maxmems.push_back(m.maxMemory);
                cout << "n=" << n << ": time=" << t_us << "us, maxMem=" << m.maxMemory << endl;
            }
            // Write traverse CSV
            string tdir = base + "/traverse/" + ggen.name;
            ensure_dir(tdir);
            string tfile = tdir + "/" + algo.name + ".csv";
            write_csv(tfile, sizes, traverse_times, maxmems);

            // Find benchmark
            find_times.clear();
            for (int n : sizes) {
                Graph g = ggen.gen(n, rng);
                double total = 0;
                uniform_int_distribution<int> dist(0, n-1);
                for (int trial = 0; trial < find_trials; ++trial) {
                    int target = dist(rng);
                    auto t1 = high_resolution_clock::now();
                    algo.find(g, 0, target);
                    auto t2 = high_resolution_clock::now();
                    total += duration_cast<microseconds>(t2 - t1).count();
                }
                double avg = total / find_trials;
                find_times.push_back(avg);
                cout << "n=" << n << ": avgFindTime=" << avg << "us" << endl;
            }
            // Write find CSV
            string fdir = base + "/find/" + ggen.name;
            ensure_dir(fdir);
            string ffile = fdir + "/" + algo.name + ".csv";
            write_find_csv(ffile, sizes, find_times);
        }
    }
    cout << "\nAll benchmarks complete. CSVs written to: " << base << endl;
    return 0;
}
