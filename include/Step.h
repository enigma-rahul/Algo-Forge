#ifndef ALGO_FORGE_STEP_H
#define ALGO_FORGE_STEP_H

#include <string>
#include <vector>
#include <map>
#include "../third_party/json.hpp"

namespace algo {

/**
 * Represents a single recorded execution event emitted during an algorithm run.
 * Serialized to JSON for consumption by the browser visualizer.
 */
struct Step {
    int step = 0;                        // 1-based step counter
    std::string type;                    // "compare", "swap", "write", "pivot", "visit", "relax", etc.
    std::vector<int> indices;            // Relevant array indices or coordinate pairs
    std::vector<int> values;             // Relevant value snapshots
    std::string message;                 // Human-readable plain-English description
    
    // Graph and Pathfinding specific fields
    int node = -1;                       // Active node ID (-1 if not applicable)
    std::vector<int> path;               // Path nodes
    std::vector<int> queue;              // Queue / Call Stack snapshot
    std::map<std::string, int> distances; // Distance map (for Dijkstra)

    // Serialization to nlohmann::json
    nlohmann::json to_json() const {
        nlohmann::json j;
        j["step"] = step;
        j["type"] = type;
        j["indices"] = indices;
        j["values"] = values;
        j["message"] = message;

        if (node != -1) {
            j["node"] = node;
        }
        if (!path.empty()) {
            j["path"] = path;
        }
        if (!queue.empty()) {
            j["queue"] = queue;
        }
        if (!distances.empty()) {
            j["distances"] = distances;
        }
        return j;
    }
};

} // namespace algo

#endif // ALGO_FORGE_STEP_H
