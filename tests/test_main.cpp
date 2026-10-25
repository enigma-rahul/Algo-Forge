#include "../include/algorithms.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <string>

// Test utility assertions with clear feedback
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[-] Assertion FAILED: " << (msg) << " (" << #cond << ") at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while (0)

template<typename T>
void printVal(std::ostream& os, const T& val) {
    os << val;
}
inline void printVal(std::ostream& os, const std::vector<int>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i] << (i + 1 < vec.size() ? ", " : "");
    }
    os << "]";
}

#define ASSERT_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "[-] Assertion FAILED: " << (msg) << " ["; \
            printVal(std::cerr, (a)); \
            std::cerr << " != "; \
            printVal(std::cerr, (b)); \
            std::cerr << "] at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while (0)

// ----------------------------------------------------------------------------
// 1. Sorting Tests
// ----------------------------------------------------------------------------
bool testSortingAlgorithms() {
    std::cout << "[*] Running Sorting Tests (Bubble, Selection, Insertion, Merge, Quick)..." << std::endl;

    std::vector<std::vector<int>> testCases = {
        {5, 2, 4, 1, 3},
        {1},
        {10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        {-5, 12, -3, 0, 8, -1, 4},
        {7, 7, 7, 7, 7},
        {1, 2, 3, 4, 5, 6}
    };

    for (const auto& original : testCases) {
        std::vector<int> expected = original;
        std::sort(expected.begin(), expected.end());

        // Test Bubble Sort
        {
            auto res = algo::sorting::bubbleSort(original);
            ASSERT_TRUE(res.success, "Bubble sort must succeed");
            std::vector<int> sorted = res.result["values"].get<std::vector<int>>();
            ASSERT_EQ(sorted, expected, "Bubble sort output must match std::sort");
            ASSERT_TRUE(!res.steps.empty(), "Bubble sort must record steps");
            ASSERT_EQ(res.steps.back().type, "done", "Bubble sort must terminate with 'done'");
            // Check indices bounds
            for (const auto& s : res.steps) {
                for (int idx : s.indices) {
                    ASSERT_TRUE(idx >= 0 && idx < static_cast<int>(original.size()), "Index within bounds");
                }
            }
        }

        // Test Selection Sort
        {
            auto res = algo::sorting::selectionSort(original);
            ASSERT_TRUE(res.success, "Selection sort must succeed");
            std::vector<int> sorted = res.result["values"].get<std::vector<int>>();
            ASSERT_EQ(sorted, expected, "Selection sort output must match std::sort");
            ASSERT_EQ(res.steps.back().type, "done", "Selection sort must terminate with 'done'");
        }

        // Test Insertion Sort
        {
            auto res = algo::sorting::insertionSort(original);
            ASSERT_TRUE(res.success, "Insertion sort must succeed");
            std::vector<int> sorted = res.result["values"].get<std::vector<int>>();
            ASSERT_EQ(sorted, expected, "Insertion sort output must match std::sort");
            ASSERT_EQ(res.steps.back().type, "done", "Insertion sort must terminate with 'done'");
        }

        // Test Merge Sort
        {
            auto res = algo::sorting::mergeSort(original);
            ASSERT_TRUE(res.success, "Merge sort must succeed");
            std::vector<int> sorted = res.result["values"].get<std::vector<int>>();
            ASSERT_EQ(sorted, expected, "Merge sort output must match std::sort");
            ASSERT_EQ(res.steps.back().type, "done", "Merge sort must terminate with 'done'");
        }

        // Test Quick Sort
        {
            auto res = algo::sorting::quickSort(original);
            ASSERT_TRUE(res.success, "Quick sort must succeed");
            std::vector<int> sorted = res.result["values"].get<std::vector<int>>();
            ASSERT_EQ(sorted, expected, "Quick sort output must match std::sort");
            ASSERT_EQ(res.steps.back().type, "done", "Quick sort must terminate with 'done'");
        }
    }

    std::cout << "  [+] All 5 Sorting Algorithms passed successfully!" << std::endl;
    return true;
}

// ----------------------------------------------------------------------------
// 2. Searching Tests
// ----------------------------------------------------------------------------
bool testSearchingAlgorithms() {
    std::cout << "[*] Running Searching Tests (Linear Search & Binary Search)..." << std::endl;

    std::vector<int> arr = {10, 20, 30, 40, 50, 60, 70};

    // Linear Search: Hit
    {
        auto res = algo::searching::linearSearch(arr, 40);
        ASSERT_TRUE(res.success, "Linear search must succeed");
        ASSERT_TRUE(res.result["found"].get<bool>(), "Linear search should find 40");
        ASSERT_EQ(res.result["index"].get<int>(), 3, "Index of 40 should be 3");
    }

    // Linear Search: Miss
    {
        auto res = algo::searching::linearSearch(arr, 99);
        ASSERT_TRUE(res.success, "Linear search must succeed");
        ASSERT_TRUE(!res.result["found"].get<bool>(), "Linear search should not find 99");
        ASSERT_EQ(res.result["index"].get<int>(), -1, "Index of missing item should be -1");
    }

    // Binary Search: Hit
    {
        auto res = algo::searching::binarySearch(arr, 50);
        ASSERT_TRUE(res.success, "Binary search must succeed on sorted input");
        ASSERT_TRUE(res.result["found"].get<bool>(), "Binary search should find 50");
        ASSERT_EQ(res.result["index"].get<int>(), 4, "Index of 50 should be 4");
    }

    // Binary Search: Miss
    {
        auto res = algo::searching::binarySearch(arr, 25);
        ASSERT_TRUE(res.success, "Binary search must succeed on sorted input");
        ASSERT_TRUE(!res.result["found"].get<bool>(), "Binary search should not find 25");
    }

    // Binary Search: Reject Unsorted Array Guard
    {
        std::vector<int> unsorted = {10, 30, 20, 40};
        auto res = algo::searching::binarySearch(unsorted, 20);
        ASSERT_TRUE(!res.success, "Binary search MUST reject unsorted array");
        ASSERT_TRUE(res.error_message.find("Binary Search requires a sorted array") != std::string::npos,
                    "Error message should explain unsorted input");
    }

    std::cout << "  [+] Linear & Binary Search tests passed successfully!" << std::endl;
    return true;
}

// ----------------------------------------------------------------------------
// 3. Graph Tests
// ----------------------------------------------------------------------------
bool testGraphAlgorithms() {
    std::cout << "[*] Running Graph Tests (BFS, DFS, Dijkstra, A*)..." << std::endl;

    std::vector<int> nodes = {0, 1, 2, 3, 4};
    std::vector<std::pair<int, int>> unweightedEdges = {
        {0, 1}, {0, 2}, {1, 3}, {2, 4}
    };

    // BFS Test
    {
        auto res = algo::graph::bfs(nodes, unweightedEdges, 0);
        ASSERT_TRUE(res.success, "BFS must succeed");
        auto order = res.result["visited_order"].get<std::vector<int>>();
        ASSERT_EQ(order.size(), 5, "BFS should visit all 5 reachable nodes");
        ASSERT_EQ(order[0], 0, "Start node should be 0");
    }

    // DFS Test
    {
        auto res = algo::graph::dfs(nodes, unweightedEdges, 0);
        ASSERT_TRUE(res.success, "DFS must succeed");
        auto order = res.result["visited_order"].get<std::vector<int>>();
        ASSERT_EQ(order.size(), 5, "DFS should visit all 5 reachable nodes");
        ASSERT_EQ(order[0], 0, "Start node should be 0");
    }

    // Dijkstra Test
    {
        std::vector<algo::graph::WeightedEdge> weightedEdges = {
            {0, 1, 4}, {0, 2, 2}, {1, 2, 1}, {1, 3, 5}, {2, 3, 8}, {2, 4, 10}, {3, 4, 2}
        };

        auto res = algo::graph::dijkstra(nodes, weightedEdges, 0, 4);
        ASSERT_TRUE(res.success, "Dijkstra must succeed on valid weights");
        int dist = res.result["shortest_distance"].get<int>();
        // Path 0 -> 2 (2) -> 1 (3) -> 3 (8) -> 4 (10)
        ASSERT_EQ(dist, 10, "Shortest path distance from 0 to 4 should be 10");
        auto path = res.result["path"].get<std::vector<int>>();
        std::vector<int> expectedPath = {0, 2, 1, 3, 4};
        ASSERT_EQ(path, expectedPath, "Shortest path vertices sequence");
    }

    // Dijkstra: Reject Negative Weights Guard
    {
        std::vector<algo::graph::WeightedEdge> badEdges = {
            {0, 1, 4}, {1, 2, -3}
        };
        auto res = algo::graph::dijkstra(nodes, badEdges, 0, 2);
        ASSERT_TRUE(!res.success, "Dijkstra MUST reject negative edge weights");
        ASSERT_TRUE(res.error_message.find("non-negative") != std::string::npos, 
                    "Error message must specify non-negative requirement");
    }

    // A* Pathfinding Test
    {
        std::vector<std::pair<int, int>> walls = {{1, 1}, {2, 1}, {3, 1}};
        auto res = algo::graph::aStar(5, 5, {0, 0}, {4, 0}, walls);
        ASSERT_TRUE(res.success, "A* must succeed");
        ASSERT_TRUE(res.result["found"].get<bool>(), "Path around obstacle wall must be found");
    }

    std::cout << "  [+] BFS, DFS, Dijkstra, and A* tests passed successfully!" << std::endl;
    return true;
}

// ----------------------------------------------------------------------------
// 4. Tree (BST) Tests
// ----------------------------------------------------------------------------
bool testTreeAlgorithms() {
    std::cout << "[*] Running Tree Tests (Binary Search Tree Insert & Search)..." << std::endl;

    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

    // Hit
    {
        auto res = algo::tree::bstInsertAndSearch(keys, 40);
        ASSERT_TRUE(res.success, "BST must succeed");
        ASSERT_TRUE(res.result["found"].get<bool>(), "Key 40 should be found in BST");
        std::vector<int> path = res.result["search_path"].get<std::vector<int>>();
        std::vector<int> expectedPath = {50, 30, 40};
        ASSERT_EQ(path, expectedPath, "Search path to 40 should be 50 -> 30 -> 40");
    }

    // Miss
    {
        auto res = algo::tree::bstInsertAndSearch(keys, 99);
        ASSERT_TRUE(res.success, "BST must succeed");
        ASSERT_TRUE(!res.result["found"].get<bool>(), "Key 99 should not be found in BST");
    }

    std::cout << "  [+] Binary Search Tree tests passed successfully!" << std::endl;
    return true;
}

// ----------------------------------------------------------------------------
// Main Test Runner
// ----------------------------------------------------------------------------
int main() {
    std::cout << "========================================================\n"
              << "  ALGO FORGE — C++ Engine Automated Test Suite\n"
              << "========================================================\n";

    bool allPassed = true;
    allPassed &= testSortingAlgorithms();
    allPassed &= testSearchingAlgorithms();
    allPassed &= testGraphAlgorithms();
    allPassed &= testTreeAlgorithms();

    std::cout << "========================================================\n";
    if (allPassed) {
        std::cout << "  [SUCCESS] All 12 algorithms and verification checks PASSED!\n";
        std::cout << "========================================================\n";
        return 0;
    } else {
        std::cerr << "  [FAILURE] One or more test suites failed.\n";
        std::cout << "========================================================\n";
        return 1;
    }
}
