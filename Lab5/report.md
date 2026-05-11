# Minimum Spanning Tree Report

## Problem overview
The goal is to compute a minimum spanning tree (MST) of a connected, weighted, undirected graph and measure the runtime of two classic algorithms (Prim and Kruskal) in both basic and optimized implementations. For each graph size, we measure the time to build an MST and export the results to CSV files for plotting.

An MST is a subset of edges that connects all vertices with no cycles and minimum total weight. Both Prim and Kruskal are proven to produce an MST when the graph is connected.

## Algorithms

### Prim's algorithm (concept)
Prim's algorithm grows a single tree starting from an arbitrary vertex. At each step, it adds the minimum-weight edge that connects any vertex inside the current tree to a vertex outside the tree. This is a greedy strategy that is correct because the lightest edge crossing any cut is always safe to include in an MST.

High-level steps:
1. Start from a chosen root (vertex 0).
2. Repeatedly pick the cheapest edge that connects the current tree to an unvisited vertex.
3. Add that edge and the new vertex to the tree.
4. Continue until all vertices are in the tree.

Prim's algorithm is particularly efficient when you can quickly find the next cheapest edge crossing the cut.

### Kruskal's algorithm (concept)
Kruskal's algorithm builds the MST by considering edges in increasing order of weight. It adds an edge if and only if it does not form a cycle with already chosen edges. This is a greedy strategy as well, based on the cut property.

High-level steps:
1. Sort all edges by weight.
2. Initialize each vertex as its own component.
3. Iterate through the sorted edges:
   - If the edge connects two different components, add it to the MST and merge the components.
   - If it connects vertices already in the same component, skip it.
4. Stop when the MST has exactly $V - 1$ edges.

The key operation here is efficient cycle detection, which is handled by a disjoint-set union (DSU) structure.

## Data structures used and why

### Graph representation
Two graph representations are used because each algorithm benefits from a different access pattern:

- **Adjacency list** (optimized Prim):
  - Represents each vertex with a list of (neighbor, weight) pairs.
  - Efficient for iterating over edges leaving a vertex.
  - Memory usage is $O(V + E)$.

- **Adjacency matrix** (basic Prim):
  - A 2D matrix of size $V \times V$ storing weights.
  - Makes it easy to find the cheapest edge by scanning all vertices.
  - Memory usage is $O(V^2)$ and each update step scans $O(V)$ neighbors.

Both representations are built from the same edge list so they always describe the same graph.

### Edge list
Kruskal's algorithm processes edges globally, so it uses a flat list of edges:
- Each edge is a triple $(u, v, w)$.
- Sorting this list is the main cost of Kruskal.

### Disjoint Set Union (DSU)
The DSU tracks connected components while Kruskal builds the MST. It supports:
- `find(x)` to get the representative of x's component.
- `unite(a, b)` to merge two components.

Two versions are used:
- **Basic DSU**: parent pointers only, no balancing.
- **Optimized DSU**: path compression + union by rank.

These optimizations reduce the amortized cost of operations to near-constant time ($\alpha(V)$).

### Priority queue (optimized Prim)
Optimized Prim uses a min-heap (priority queue) of pairs `(key, vertex)`:
- The key is the current cheapest known edge connecting that vertex to the tree.
- This allows extracting the next cheapest candidate in $O(\log V)$ time.

This replaces the $O(V)$ scan used in the basic version.

## How the algorithms and data structures work together

### Prim (basic)
- Builds a dense adjacency matrix for direct weight lookups.
- Maintains arrays:
  - `key[v]`: cheapest edge weight that can connect v to the tree.
  - `parent[v]`: which vertex provides that cheapest connection.
  - `inTree[v]`: whether v is already in the MST.
- Each iteration:
  1. Scan all vertices to pick the minimum `key` among those not in the tree.
  2. Update `key` and `parent` for all other vertices by scanning the matrix row.

This is simple but costs $O(V^2)$ time.

### Prim (optimized)
- Uses an adjacency list and a min-heap.
- The heap always stores the next candidate edge by weight.
- Each time the minimum entry is popped:
  - If the vertex is not yet in the tree, it is added.
  - Neighbor keys are updated and pushed into the heap.

The complexity becomes $O(E \log V)$, which is a big improvement for sparse graphs.

### Kruskal (basic and optimized)
- Uses the edge list and sorts it once by weight ($O(E \log E)$).
- Iterates in ascending order and uses DSU to prevent cycles.
- The MST grows by selecting the lightest edges that connect different components.

The only difference between basic and optimized Kruskal is the DSU implementation.

## Optimizations and why they matter

### Optimized Prim
- **Adjacency list instead of matrix**:
  - Avoids scanning non-edges.
  - For sparse graphs, $E$ is much smaller than $V^2$, so this saves a lot of work.
- **Priority queue for minimum key selection**:
  - Replaces the $O(V)$ scan per iteration with $O(\log V)$ extraction.
  - Total complexity reduces from $O(V^2)$ to $O(E \log V)$.

### Optimized Kruskal
- **Path compression in DSU**:
  - Flattens the tree structure during `find`, making future finds faster.
- **Union by rank**:
  - Attaches smaller trees under larger ones to keep DSU trees shallow.
- Combined effect: DSU operations are almost constant time, making the total runtime dominated by the sort ($O(E \log E)$).

### Benchmark improvements (measurement accuracy)
- Each algorithm is run multiple times per graph size and averaged.
- This reduces noise from CPU scheduling and cache effects.

## Why Prim is consistently slower than Kruskal here
The empirical results show Prim slower than Kruskal for the chosen graph sizes and densities. The reasons are mostly about data access patterns and constant factors:

1. **Graph density is low (sparse graphs)**
   - With $E \approx V(1 + \text{extraEdgesPerNode})$, the graphs are sparse.
   - Kruskal runs in $O(E \log E)$ and benefits from a relatively small edge list.
   - Basic Prim uses an adjacency matrix, which forces $O(V^2)$ work even when few edges exist.

2. **Heap overhead in optimized Prim**
   - Optimized Prim avoids the $O(V^2)$ scan, but it still performs many heap pushes and pops.
   - Each update can push a new key, and old keys become stale, increasing heap traffic.
   - Kruskal does one sort and then fast DSU checks, which often has lower constant factors.

3. **Sorting vs repeated key updates**
   - Kruskal pays a one-time cost to sort edges, then performs near-constant-time unions.
   - Prim continually updates keys and reorders the priority queue.

4. **Cache behavior**
   - Kruskal's edge list is contiguous and cache-friendly after sorting.
   - Prim's heap operations involve more scattered memory access.

For dense graphs, the story can change. With $E$ close to $V^2$, Prim with a matrix can be competitive or faster than Kruskal because $E \log E$ grows faster than $V^2$. In this project, graphs are sparse, so Kruskal tends to win.

## Summary
- Both algorithms are correct and rely on greedy choices backed by the cut property.
- Kruskal depends on sorting and DSU; Prim depends on fast access to the next cheapest boundary edge.
- Optimizations reduce the asymptotic cost and constant factors, explaining the big timing gap.
- In the current sparse-graph setup, Kruskal is consistently faster than Prim.
