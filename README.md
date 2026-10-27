# Algo Forge — Interactive C++ Algorithm Visualizer

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()

**Algo Forge** is an educational, browser-based algorithm visualizer that demonstrates real algorithms implemented in **C++17**.

Unlike traditional visualizers where JavaScript simulates the algorithm steps in the browser, **Algo Forge** executes all algorithmic logic strictly in modern C++. A C++ `StepRecorder` logs discrete, typed execution events (comparisons, swaps, pivots, visits, queue changes, relaxation, and tree branches). A local C++ HTTP server exposes this via a clean JSON API and serves the browser UI, which replays the C++ execution trace with rich playback controls.

---

## Architecture

```text
       Browser UI (HTML5 / Vanilla CSS / Vanilla JS)
                     │
                     │ POST /api/visualize  (JSON Request)
                     ▼
  C++ Embedded HTTP Server (cpp-httplib on Port 8080)
                     │
                     ├── Validates Input & Dispatches Algorithm
                     ▼
             C++ Algorithm Engine (12 Algorithms)
                     │
                     ├── StepRecorder logs events (compare, swap, visit, etc.)
                     ▼
             AlgorithmResult (C++17 Struct)
                     │
                     │ JSON Response (Input + Result + Steps)
                     ▼
       Browser UI Replays C++ Event Trace
       (Array Bars, 2D Graph Canvas/SVG, BST Tree)
```

### Key Highlights
- **12 Algorithms Implemented**: 5 Sorting, 2 Searching, 4 Graph/Pathfinding, and 1 Binary Search Tree.
- **Strict Separation of Concerns**: JavaScript does **not** implement or duplicate any algorithm logic. It functions strictly as a playback controller and renderer.
- **Robust Input Validation**: Strict guards with helpful feedback (e.g. Binary Search rejects unsorted arrays; Dijkstra rejects negative edge weights).
- **Zero Package Manager Setup**: Header-only dependencies (`nlohmann/json.hpp` and `httplib.h`) are vendored in `third_party/`. Builds cleanly with standard Apple Clang or GCC.
- **Single Process**: The C++ server hosts both the static frontend and the API, avoiding CORS configuration.

---
## Algorithm Catalog & Complexities

| Category | Algorithm | Best Time | Average Time | Worst Time | Auxiliary Space |
|---|---|---|---|---|---|
| Sorting | Bubble Sort | O(n)* | O(n²) | O(n²) | O(1) |
| Sorting | Selection Sort | O(n²) | O(n²) | O(n²) | O(1) |
| Sorting | Insertion Sort | O(n) | O(n²) | O(n²) | O(1) |
| Sorting | Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Sorting | Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) average; O(n) worst |
| Searching | Linear Search | O(1) | O(n) | O(n) | O(1) |
| Searching | Binary Search | O(1) | O(log n) | O(log n) | O(1) iterative |
| Graph | BFS | O(V + E) | O(V + E) | O(V + E) | O(V) |
| Graph | DFS | O(V + E) | O(V + E) | O(V + E) | O(V) |
| Graph | Dijkstra | O((V + E) log V) | O((V + E) log V) | O((V + E) log V) | O(V) |
| Pathfinding | A* | Implementation-dependent | Heuristic-dependent | Can be exponential in search-tree formulations | O(V) for a graph-search implementation |
| Tree | BST Search/Insertion | O(1) best case | O(log n) for a suitably balanced tree | O(n) | O(h) auxiliary space for recursive operations |

*Bubble Sort achieves best-case O(n) only when the implementation stops early if a pass makes no swaps.*

Here, n is the number of elements, V is the number of graph vertices, E is the number of graph edges, and h is the height of the tree.

These complexities assume conventional implementations; verify them against your actual code. Dijkstra's stated bound assumes a suitable priority queue and graph representation.

---
## Quickstart & Build Instructions

### Prerequisites
- macOS (Apple Silicon or Intel) or Linux
- A C++17 compatible compiler: `clang++` (Xcode Command Line Tools: `xcode-select --install`) or `g++` (version 9+)
- `make` or `cmake` (version 3.14+)

### Option A: Build with Make (Recommended)

```bash
# 1. Clone the repository
git clone https://github.com/enigma-rahul/Algo-Forge.git
cd algo-forge

# 2. Build both server and tests
make all

# 3. Run the automated test suite
make test

# 4. Start the server
make run
```

### Option B: Build with CMake

```bash
# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run tests
./build/algo_tests

# Run server
./build/algo_forge_server --port 8080 --web ./web
```

Open your browser to: **`http://localhost:8080`**

---

## API Documentation

### 1. Health Check
`GET /api/health`

**Example Request:**
```bash
curl -s http://localhost:8080/api/health
```

**Response (200 OK):**
```json
{
  "status": "ok",
  "version": "1.0.0",
  "engine": "C++17",
  "algorithms": [
    "bubble_sort", "selection_sort", "insertion_sort", "merge_sort", "quick_sort",
    "linear_search", "binary_search", "bfs", "dfs", "dijkstra", "astar", "bst"
  ]
}
```

---

### 2. Run Algorithm & Record Steps
`POST /api/visualize`

#### A. Array Sorting Example: Quick Sort
```bash
curl -s -X POST http://localhost:8080/api/visualize \
  -H "Content-Type: application/json" \
  -d '{
    "algorithm": "quick_sort",
    "input": {
      "values": [40, 20, 70, 10, 90, 30]
    }
  }'
```

**Response (200 OK):**
```json
{
  "algorithm": "quick_sort",
  "input": { "values": [40, 20, 70, 10, 90, 30] },
  "metadata": {
    "comparisons": 11,
    "step_count": 19,
    "swaps": 5
  },
  "result": { "values": [10, 20, 30, 40, 70, 90] },
  "steps": [
    {
      "indices": [5],
      "message": "Chosen pivot: arr[5] = 30",
      "step": 1,
      "type": "pivot",
      "values": [30]
    },
    {
      "indices": [0, 5],
      "message": "Comparing arr[0] (40) and arr[5] (30)",
      "step": 2,
      "type": "compare",
      "values": [40, 30]
    }
  ]
}
```

#### B. Graph Dijkstra Example
```bash
curl -s -X POST http://localhost:8080/api/visualize \
  -H "Content-Type: application/json" \
  -d '{
    "algorithm": "dijkstra",
    "input": {
      "nodes": [0, 1, 2, 3],
      "edges": [[0, 1, 4], [0, 2, 2], [2, 1, 1], [1, 3, 3]],
      "start": 0,
      "target": 3
    }
  }'
```

**Response (200 OK):**
```json
{
  "algorithm": "dijkstra",
  "result": {
    "path": [0, 2, 1, 3],
    "shortest_distance": 6
  },
  "steps": [...]
}
```

#### C. Validation Error Example: Unsorted Binary Search
```bash
curl -s -i -X POST http://localhost:8080/api/visualize \
  -H "Content-Type: application/json" \
  -d '{
    "algorithm": "binary_search",
    "input": {
      "values": [10, 30, 20, 40],
      "target": 20
    }
  }'
```

**Response (HTTP 400 Bad Request):**
```json
{
  "error": "Execution Validation Error",
  "message": "Binary Search requires a sorted array. Element at index 2 (20) is smaller than preceding element at index 1 (30)."
}
```

---

## 👥 Project Team

| Member | Role | Primary Responsibilities |
|---|---|---|
| **Rahul Upadhyay** | Admin & Integration | Server, API contract, frontend integration, build configuration, documentation, and integration testing |
| **Rahul Kumar Saw** | Algorithm Developer | Sorting and searching algorithms |
| **Radheshyam Chaudhary** | Algorithm Developer | Graph, pathfinding, and tree algorithms |

The team worked on separate modules that integrate into a unified C++17-powered algorithm visualization platform.


## Project Structure

```text
algo-forge/
├── CMakeLists.txt              # Standard CMake build definition
├── Makefile                    # Ergonomic student/grader build driver
├── README.md                   # Complete documentation & demo guide
├── third_party/
│   ├── json.hpp                # nlohmann/json single header v3.11.3
│   └── httplib.h               # cpp-httplib single header v0.18.3
├── include/
│   ├── Step.h                  # Step data structure & JSON serialization
│   ├── StepRecorder.h          # Safe event recorder with step bounds
│   ├── AlgorithmResult.h       # Result and metrics wrapper
│   ├── algorithms.h            # Master algorithms header
│   ├── algorithms/
│   │   ├── sorting.h           # Bubble, Selection, Insertion, Merge, Quick
│   │   ├── searching.h         # Linear Search, Binary Search
│   │   ├── graph.h             # BFS, DFS, Dijkstra, A*
│   │   └── tree.h              # BST Insert & Search
│   └── server.h                # Embedded HTTP server interface
├── src/
│   ├── main.cpp                # CLI entry point (--port, --web, --host)
│   ├── server.cpp              # Endpoint validation & request dispatch
│   ├── StepRecorder.cpp        # Step recording implementation
│   └── algorithms/
│       ├── sorting.cpp         # 5 Sorting implementations
│       ├── searching.cpp       # 2 Searching implementations
│       ├── graph.cpp           # 4 Graph implementations
│       └── tree.cpp            # BST implementation
├── tests/
│   └── test_main.cpp           # Automated test suite (all 12 algorithms)
└── web/
    ├── index.html              # Modern single-page app HTML
    ├── styles.css              # Custom dark-theme design system
    └── app.js                  # Frontend playback & rendering controller
```

---

## License
MIT License. Built for educational demonstration and portfolio presentation.
