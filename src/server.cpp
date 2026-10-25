#include "../include/server.h"
#include "../include/algorithms.h"
#include "../third_party/httplib.h"
#include "../third_party/json.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using json = nlohmann::json;

namespace algo {
namespace server {

namespace {

void sendJsonError(httplib::Response& res, int statusCode, const std::string& errType, const std::string& message) {
    json err;
    err["error"] = errType;
    err["message"] = message;
    res.status = statusCode;
    res.set_content(err.dump(), "application/json");
}

AlgorithmResult dispatchAlgorithm(const std::string& name, const json& input) {
    // 1. Sorting Algorithms
    if (name == "bubble_sort" || name == "selection_sort" || 
        name == "insertion_sort" || name == "merge_sort" || name == "quick_sort") {
        
        if (!input.contains("values") || !input["values"].is_array()) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "Sorting algorithms require 'input.values' to be an array of integers.";
            return err;
        }

        std::vector<int> arr = input["values"].get<std::vector<int>>();
        if (arr.size() < 1) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "Array must contain at least 1 element.";
            return err;
        }
        if (arr.size() > 100) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "Array exceeds maximum allowed size (100 elements).";
            return err;
        }

        if (name == "bubble_sort") return sorting::bubbleSort(arr);
        if (name == "selection_sort") return sorting::selectionSort(arr);
        if (name == "insertion_sort") return sorting::insertionSort(arr);
        if (name == "merge_sort") return sorting::mergeSort(arr);
        if (name == "quick_sort") return sorting::quickSort(arr);
    }

    // 2. Searching Algorithms
    if (name == "linear_search" || name == "binary_search") {
        if (!input.contains("values") || !input["values"].is_array()) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "Search algorithms require 'input.values' array.";
            return err;
        }
        if (!input.contains("target") || !input["target"].is_number_integer()) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "Search algorithms require an integer 'input.target'.";
            return err;
        }

        std::vector<int> arr = input["values"].get<std::vector<int>>();
        int target = input["target"].get<int>();

        if (name == "linear_search") return searching::linearSearch(arr, target);
        if (name == "binary_search") return searching::binarySearch(arr, target);
    }

    // 3. Graph Algorithms
    if (name == "bfs" || name == "dfs") {
        if (!input.contains("nodes") || !input["nodes"].is_array() ||
            !input.contains("edges") || !input["edges"].is_array() ||
            !input.contains("start") || !input["start"].is_number_integer()) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "Graph traversal requires 'nodes' array, 'edges' array, and 'start' integer.";
            return err;
        }

        std::vector<int> nodes = input["nodes"].get<std::vector<int>>();
        int start = input["start"].get<int>();

        std::vector<std::pair<int, int>> edges;
        for (const auto& item : input["edges"]) {
            if (!item.is_array() || item.size() < 2) {
                AlgorithmResult err;
                err.success = false;
                err.error_message = "Each edge in graph must be an array of [u, v].";
                return err;
            }
            edges.push_back({item[0].get<int>(), item[1].get<int>()});
        }

        if (name == "bfs") return graph::bfs(nodes, edges, start);
        if (name == "dfs") return graph::dfs(nodes, edges, start);
    }

    // 4. Dijkstra Shortest Path
    if (name == "dijkstra") {
        if (!input.contains("nodes") || !input["nodes"].is_array() ||
            !input.contains("edges") || !input["edges"].is_array() ||
            !input.contains("start") || !input["start"].is_number_integer() ||
            !input.contains("target") || !input["target"].is_number_integer()) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "Dijkstra requires 'nodes', 'edges', 'start', and 'target'.";
            return err;
        }

        std::vector<int> nodes = input["nodes"].get<std::vector<int>>();
        int start = input["start"].get<int>();
        int target = input["target"].get<int>();

        std::vector<graph::WeightedEdge> edges;
        for (const auto& item : input["edges"]) {
            if (!item.is_array() || item.size() < 3) {
                AlgorithmResult err;
                err.success = false;
                err.error_message = "Dijkstra edges must have format [u, v, weight].";
                return err;
            }
            edges.push_back({item[0].get<int>(), item[1].get<int>(), item[2].get<int>()});
        }

        return graph::dijkstra(nodes, edges, start, target);
    }

    // 5. A* Pathfinding (Stretch)
    if (name == "astar") {
        int rows = input.value("rows", 6);
        int cols = input.value("cols", 8);

        std::pair<int, int> start = {0, 0};
        if (input.contains("start") && input["start"].is_array() && input["start"].size() >= 2) {
            start = {input["start"][0].get<int>(), input["start"][1].get<int>()};
        }

        std::pair<int, int> target = {rows - 1, cols - 1};
        if (input.contains("target") && input["target"].is_array() && input["target"].size() >= 2) {
            target = {input["target"][0].get<int>(), input["target"][1].get<int>()};
        }

        std::vector<std::pair<int, int>> walls;
        if (input.contains("walls") && input["walls"].is_array()) {
            for (const auto& w : input["walls"]) {
                if (w.is_array() && w.size() >= 2) {
                    walls.push_back({w[0].get<int>(), w[1].get<int>()});
                }
            }
        }

        return graph::aStar(rows, cols, start, target, walls);
    }

    // 6. Binary Search Tree (Stretch)
    if (name == "bst") {
        if (!input.contains("keys") || !input["keys"].is_array() ||
            !input.contains("search_target") || !input["search_target"].is_number_integer()) {
            AlgorithmResult err;
            err.success = false;
            err.error_message = "BST requires 'keys' array and 'search_target' integer.";
            return err;
        }

        std::vector<int> keys = input["keys"].get<std::vector<int>>();
        int target = input["search_target"].get<int>();
        return tree::bstInsertAndSearch(keys, target);
    }

    AlgorithmResult unknown;
    unknown.success = false;
    unknown.error_message = "Unknown algorithm: '" + name + "'.";
    return unknown;
}

} // anonymous namespace

void runServer(const std::string& host, int port, const std::string& webDir) {
    httplib::Server svr;

    // CORS and OPTIONS handling
    svr.set_pre_routing_handler([](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        if (req.method == "OPTIONS") {
            res.status = 204;
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });

    // 1. Health check route
    svr.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
        json health;
        health["status"] = "ok";
        health["version"] = "1.0.0";
        health["engine"] = "C++17";
        health["algorithms"] = {
            "bubble_sort", "selection_sort", "insertion_sort", "merge_sort", "quick_sort",
            "linear_search", "binary_search", "bfs", "dfs", "dijkstra", "astar", "bst"
        };
        res.set_content(health.dump(2), "application/json");
    });

    // 2. Algorithm visualization execution route
    svr.Post("/api/visualize", [](const httplib::Request& req, httplib::Response& res) {
        json reqJson;
        try {
            reqJson = json::parse(req.body);
        } catch (const json::parse_error& e) {
            sendJsonError(res, 400, "Malformed JSON", e.what());
            return;
        }

        if (!reqJson.contains("algorithm") || !reqJson["algorithm"].is_string()) {
            sendJsonError(res, 400, "Validation Error", "Missing or invalid 'algorithm' field.");
            return;
        }
        if (!reqJson.contains("input") || !reqJson["input"].is_object()) {
            sendJsonError(res, 400, "Validation Error", "Missing or invalid 'input' object.");
            return;
        }

        std::string algoName = reqJson["algorithm"].get<std::string>();
        AlgorithmResult result = dispatchAlgorithm(algoName, reqJson["input"]);

        if (!result.success) {
            sendJsonError(res, 400, "Execution Validation Error", result.error_message);
            return;
        }

        res.status = 200;
        res.set_content(result.to_json().dump(), "application/json");
    });

    // 3. Static frontend file hosting
    svr.set_mount_point("/", webDir);

    std::cout << "========================================================\n"
              << "  ALGO FORGE — Interactive C++ Algorithm Server\n"
              << "========================================================\n"
              << "  Listening on : http://" << host << ":" << port << "\n"
              << "  Web directory: " << webDir << "\n"
              << "  API Health   : http://" << host << ":" << port << "/api/health\n"
              << "  Supported    : 12 Algorithms (Sorting, Search, Graph, Tree)\n"
              << "========================================================\n"
              << "Press Ctrl+C to stop.\n\n";

    if (!svr.listen(host, port)) {
        std::cerr << "Failed to bind to " << host << ":" << port << ". Try specifying a different port.\n";
    }
}

} // namespace server
} // namespace algo
