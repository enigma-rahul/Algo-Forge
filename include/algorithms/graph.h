#ifndef ALGO_FORGE_GRAPH_H
#define ALGO_FORGE_GRAPH_H

#include <vector>
#include <utility>
#include "../AlgorithmResult.h"

namespace algo {
namespace graph {

// Traversal & Shortest Path
AlgorithmResult bfs(const std::vector<int>& nodes, 
                    const std::vector<std::pair<int, int>>& edges, 
                    int startNode);

AlgorithmResult dfs(const std::vector<int>& nodes, 
                    const std::vector<std::pair<int, int>>& edges, 
                    int startNode);

struct WeightedEdge {
    int u;
    int v;
    int weight;
};

AlgorithmResult dijkstra(const std::vector<int>& nodes, 
                        const std::vector<WeightedEdge>& edges, 
                        int startNode, 
                        int targetNode);

// Stretch Target: A* Grid Pathfinding
AlgorithmResult aStar(int rows, int cols, 
                      std::pair<int, int> start, 
                      std::pair<int, int> target, 
                      const std::vector<std::pair<int, int>>& walls);

} // namespace graph
} // namespace algo

#endif // ALGO_FORGE_GRAPH_H
