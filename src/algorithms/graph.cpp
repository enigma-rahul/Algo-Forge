#include "../../include/algorithms/graph.h"
#include "../../include/StepRecorder.h"
#include <queue>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include <sstream>

namespace algo {
namespace graph {

// ----------------------------------------------------------------------------
// 1. Breadth-First Search (BFS)
// ----------------------------------------------------------------------------
AlgorithmResult bfs(const std::vector<int>& nodes, 
                    const std::vector<std::pair<int, int>>& edges, 
                    int startNode) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "bfs";

    nlohmann::json edgesJson = nlohmann::json::array();
    for (const auto& e : edges) edgesJson.push_back({e.first, e.second});
    res.input = {{"nodes", nodes}, {"edges", edgesJson}, {"start", startNode}};

    // Build adjacency list (undirected)
    std::map<int, std::vector<int>> adj;
    for (int node : nodes) adj[node] = {};
    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }
    // Sort adjacency lists for deterministic traversal
    for (auto& pair : adj) {
        std::sort(pair.second.begin(), pair.second.end());
    }

    std::set<int> visited;
    std::vector<int> queue;
    std::vector<int> visitOrder;

    queue.push_back(startNode);
    visited.insert(startNode);
    recorder.recordEnqueue(startNode, queue, "Enqueued start node into BFS queue");

    while (!queue.empty()) {
        int u = queue.front();
        queue.erase(queue.begin());
        recorder.recordDequeue(u, queue, "Dequeued node for layer exploration");

        recorder.recordVisit(u, "Explored node and visiting neighbors");
        visitOrder.push_back(u);

        for (int v : adj[u]) {
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                queue.push_back(v);
                recorder.recordEnqueue(v, queue, "Discovered and enqueued unvisited neighbor");
            }
        }
    }

    recorder.recordDone(visitOrder, "BFS traversal complete");
    res.result = {{"visited_order", visitOrder}};
    res.steps = recorder.getSteps();
    return res;
}

// ----------------------------------------------------------------------------
// 2. Depth-First Search (DFS)
// ----------------------------------------------------------------------------
namespace {
void dfsHelper(int u, 
               const std::map<int, std::vector<int>>& adj, 
               std::set<int>& visited, 
               std::vector<int>& callStack, 
               std::vector<int>& visitOrder, 
               StepRecorder& recorder) {
    visited.insert(u);
    callStack.push_back(u);
    recorder.recordPush(u, callStack, "Pushed node onto DFS recursion stack");
    recorder.recordVisit(u, "Visiting node");
    visitOrder.push_back(u);

    auto it = adj.find(u);
    if (it != adj.end()) {
        for (int v : it->second) {
            if (visited.find(v) == visited.end()) {
                dfsHelper(v, adj, visited, callStack, visitOrder, recorder);
            }
        }
    }

    callStack.pop_back();
    recorder.recordPop(u, callStack, "Finished exploring all branches, backtracking");
}
} // anonymous namespace

AlgorithmResult dfs(const std::vector<int>& nodes, 
                    const std::vector<std::pair<int, int>>& edges, 
                    int startNode) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "dfs";

    nlohmann::json edgesJson = nlohmann::json::array();
    for (const auto& e : edges) edgesJson.push_back({e.first, e.second});
    res.input = {{"nodes", nodes}, {"edges", edgesJson}, {"start", startNode}};

    std::map<int, std::vector<int>> adj;
    for (int node : nodes) adj[node] = {};
    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }
    for (auto& pair : adj) {
        std::sort(pair.second.begin(), pair.second.end());
    }

    std::set<int> visited;
    std::vector<int> callStack;
    std::vector<int> visitOrder;

    dfsHelper(startNode, adj, visited, callStack, visitOrder, recorder);

    recorder.recordDone(visitOrder, "DFS traversal complete");
    res.result = {{"visited_order", visitOrder}};
    res.steps = recorder.getSteps();
    return res;
}

// ----------------------------------------------------------------------------
// 3. Dijkstra's Algorithm
// ----------------------------------------------------------------------------
AlgorithmResult dijkstra(const std::vector<int>& nodes, 
                        const std::vector<WeightedEdge>& edges, 
                        int startNode, 
                        int targetNode) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "dijkstra";

    nlohmann::json edgesJson = nlohmann::json::array();
    for (const auto& e : edges) {
        // Requirement: Validate non-negative edge weights
        if (e.weight < 0) {
            res.success = false;
            std::ostringstream err;
            err << "Dijkstra's algorithm requires non-negative edge weights. Negative weight detected on edge ("
                << e.u << " -> " << e.v << ", weight = " << e.weight << ").";
            res.error_message = err.str();
            return res;
        }
        edgesJson.push_back({e.u, e.v, e.weight});
    }

    res.input = {
        {"nodes", nodes},
        {"edges", edgesJson},
        {"start", startNode},
        {"target", targetNode}
    };

    // Build weighted adjacency list (undirected)
    std::map<int, std::vector<std::pair<int, int>>> adj; // u -> [(v, weight)]
    for (int node : nodes) adj[node] = {};
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.weight});
        adj[e.v].push_back({e.u, e.weight});
    }

    const int INF = 999999;
    std::map<int, int> dist;
    std::map<int, int> parent;
    std::map<std::string, int> distStrMap;

    for (int node : nodes) {
        dist[node] = INF;
        distStrMap[std::to_string(node)] = INF;
        parent[node] = -1;
    }
    dist[startNode] = 0;
    distStrMap[std::to_string(startNode)] = 0;

    // Min-priority queue: (distance, node)
    using PII = std::pair<int, int>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
    pq.push({0, startNode});

    std::set<int> finalized;

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (finalized.find(u) != finalized.end()) continue;
        finalized.insert(u);

        recorder.recordVisit(u, "Finalized shortest distance to node");

        if (u == targetNode) break;

        for (const auto& [v, weight] : adj[u]) {
            if (finalized.find(v) == finalized.end() && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                distStrMap[std::to_string(v)] = dist[v];
                parent[v] = u;
                pq.push({dist[v], v});
                recorder.recordRelax(u, v, weight, dist[v], distStrMap);
            }
        }
    }

    // Reconstruct path
    std::vector<int> path;
    if (dist[targetNode] != INF) {
        int curr = targetNode;
        while (curr != -1) {
            path.push_back(curr);
            curr = parent[curr];
        }
        std::reverse(path.begin(), path.end());
        recorder.recordPath(path, "Target reached: shortest path found!");
    } else {
        recorder.recordPath({}, "Target node is unreachable from start node");
    }

    recorder.recordDone(path, "Dijkstra execution finished");
    res.result = {
        {"shortest_distance", dist[targetNode] == INF ? -1 : dist[targetNode]},
        {"path", path}
    };
    res.steps = recorder.getSteps();
    return res;
}

// ----------------------------------------------------------------------------
// 4. A* Grid Pathfinding (Stretch Target)
// ----------------------------------------------------------------------------
namespace {
int manhattanDist(int r1, int c1, int r2, int c2) {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}
} // anonymous namespace

AlgorithmResult aStar(int rows, int cols, 
                      std::pair<int, int> start, 
                      std::pair<int, int> target, 
                      const std::vector<std::pair<int, int>>& walls) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "astar";

    nlohmann::json wallsJson = nlohmann::json::array();
    for (const auto& w : walls) wallsJson.push_back({w.first, w.second});
    res.input = {
        {"rows", rows},
        {"cols", cols},
        {"start", {start.first, start.second}},
        {"target", {target.first, target.second}},
        {"walls", wallsJson}
    };

    std::set<std::pair<int, int>> wallSet(walls.begin(), walls.end());

    struct Cell {
        int r, c;
        int g, h, f;
        bool operator>(const Cell& other) const { return f > other.f; }
    };

    std::map<std::pair<int, int>, int> gScore;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
    std::set<std::pair<int, int>> closedSet;

    std::priority_queue<Cell, std::vector<Cell>, std::greater<Cell>> openSet;

    int startH = manhattanDist(start.first, start.second, target.first, target.second);
    openSet.push({start.first, start.second, 0, startH, startH});
    gScore[start] = 0;
    recorder.recordGridOpen(start.first, start.second, startH, 0, startH, "Placed start cell in open set");

    bool found = false;
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while (!openSet.empty()) {
        Cell curr = openSet.top();
        openSet.pop();

        std::pair<int, int> currPt = {curr.r, curr.c};
        if (closedSet.find(currPt) != closedSet.end()) continue;
        closedSet.insert(currPt);

        recorder.recordGridClose(curr.r, curr.c);

        if (currPt == target) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];
            std::pair<int, int> nextPt = {nr, nc};

            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (wallSet.find(nextPt) != wallSet.end()) continue;
            if (closedSet.find(nextPt) != closedSet.end()) continue;

            int tentativeG = curr.g + 1;
            if (gScore.find(nextPt) == gScore.end() || tentativeG < gScore[nextPt]) {
                cameFrom[nextPt] = currPt;
                gScore[nextPt] = tentativeG;
                int h = manhattanDist(nr, nc, target.first, target.second);
                int f = tentativeG + h;
                openSet.push({nr, nc, tentativeG, h, f});
                recorder.recordGridOpen(nr, nc, f, tentativeG, h);
            }
        }
    }

    std::vector<std::pair<int, int>> path;
    if (found) {
        std::pair<int, int> curr = target;
        while (curr != start) {
            path.push_back(curr);
            curr = cameFrom[curr];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        recorder.recordGridPath(path, "Shortest path reconstructed using A* heuristic");
    }

    nlohmann::json pathJson = nlohmann::json::array();
    for (const auto& p : path) pathJson.push_back({p.first, p.second});

    recorder.recordDone({}, found ? "A* pathfinding completed: target reached!" : "Target unreachable");
    res.result = {{"found", found}, {"path", pathJson}};
    res.steps = recorder.getSteps();
    return res;
}

} // namespace graph
} // namespace algo
