# ⚒️ AlgoForge

### Interactive Algorithm & Data Structure Visualization Platform

**Understand algorithms by watching them work.**

AlgoForge is a C++-powered educational platform designed to help students understand algorithms and data structures through interactive, step-by-step visualizations. Users can observe how algorithms process data, compare elements, swap values, and reach their final results.

The project combines a C++ algorithm engine with a lightweight HTTP API and a web-based visualization interface built using HTML, CSS, and JavaScript.

> **Project status:** Core Roadmap Completed ✅ — Enhancements Ongoing

---

## 🎯 Project Objectives

- Visualize algorithms step by step to make complex concepts easier to understand.
- Implement algorithms and data structures in C++.
- Record algorithm operations, including comparisons, swaps, and state changes.
- Connect the C++ engine to a browser-based frontend through a JSON API.
- Help students explore algorithm behavior, time complexity, and performance.
- Maintain a modular, extensible architecture for future improvements.

## 👥 Project Collaborators

- **Radheshyam Chaudhary**
- **Rahul Kumar Saw**

## ✨ Features

### 1. Interactive Algorithm Visualization

- Visual representation of array operations.
- Step-by-step execution.
- Play, pause, next-step, and reset controls.
- Adjustable animation speed.
- Highlighting of elements being compared or swapped.

### 2. C++ Algorithm Engine

- C++ implementations of algorithms and data structures.
- Modular architecture for adding new algorithms.
- A custom `StepRecorder` for capturing execution events.
- Execution statistics, including comparisons and swaps.

### 3. Algorithm Learning

- Algorithm explanations and complexity information.
- Best-case, average-case, and worst-case time complexity where applicable.
- Space complexity information.
- Opportunities to compare different algorithms.

### 4. Web-Based Interface

- Browser-based algorithm visualizations.
- Array input and example datasets.
- Execution controls and statistics.
- Responsive interface for different screen sizes.

*Keep this feature list synchronized with the functionality verified in the current codebase.*

## 🧠 Algorithms

Sorting

- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort

Searching

- Linear Search
- Binary Search

Graph Algorithms

- Breadth First Search (BFS)
- Depth First Search (DFS)
- Dijkstra's Algorithm
  
## 🏗️ System Architecture

```text
                  AlgoForge
                      |
          +-----------+-----------+
          |                       |
     C++ Backend              Frontend
          |                       |
   Algorithm Engine         HTML / CSS / JS
          |
     StepRecorder
          |
   Execution Statistics
          |
      C++ HTTP API
          |
       JSON Data
          |
          +------> Visualization UI
```

### How it works

1. **User input:** The user selects an algorithm and provides an array.
2. **C++ execution:** The algorithm engine processes the input.
3. **Step recording:** `StepRecorder` captures important operations and relevant states.
4. **API communication:** The backend exchanges execution data with the frontend through JSON.
5. **Visualization:** JavaScript animates the recorded steps in the browser.
6. **Analysis:** The interface presents execution statistics and complexity information.

The algorithm logic resides in C++, while JavaScript handles visualization and browser interaction.

## 🛠️ Technology Stack

| Technology | Purpose |
|---|---|
| C++ | Algorithm engine, data structures, and execution tracking |
| C++ HTTP library | Backend API and HTTP communication |
| JSON | Data exchange between backend and frontend |
| HTML5 | Web page structure |
| CSS3 | Styling and responsive layouts |
| JavaScript | Visualization, animations, and user interactions |
| Git & GitHub | Version control and collaboration |
| CMake | C++ build configuration |

## 📂 Project Structure

```text
AlgoForge/
├── backend/
│   ├── algorithms/
│   │   ├── bubble_sort.cpp
│   │   └── quick_sort.cpp
│   ├── core/
│   │   ├── step.h
│   │   └── step_recorder.h
│   ├── api/
│   ├── datastructures/
│   └── main.cpp
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── app.js
├── tests/
├── CMakeLists.txt
├── .gitignore
└── README.md
```

*This structure illustrates the intended organization; adjust it to match the actual repository.*

## 🗺️ Six-Week Development Roadmap

- [x] **Week 1 — C++ Core:** Algorithm engine, Bubble Sort, Quick Sort, and `StepRecorder`.
- [x] **Week 2 — Backend API:** C++ HTTP server and JSON request/response handling.
- [x] **Week 3 — Frontend:** Web interface and backend integration.
- [x] **Week 4 — Visualization:** Step playback, animations, speed controls, and execution statistics.
- [x] **Week 5 — Expansion:** Additional algorithms, explanations, and edge-case testing.
- [x] **Week 6 — Finalization:** Bug fixes, UI refinement, documentation, and project demonstration.

## 🚀 Getting Started

Use the following prerequisites to build and run the project:

- A compatible C++ compiler.
- CMake.
- Git.
- A modern web browser.
- The C++ HTTP and JSON libraries selected by the project.

Add the verified installation, build, and run commands for your repository here so other developers can reproduce the setup.

## 🔮 Future Improvements

- Additional sorting algorithms, including Heap Sort and Merge Sort.
- Searching algorithms and tree visualizations.
- Stack, queue, linked-list, and graph visualizations.
- Side-by-side algorithm comparisons.
- Code explanations synchronized with execution steps.
- Curated learning resources and tutorial links.

## 🎓 Why AlgoForge?

Many learners memorize algorithm code without fully understanding how it works. AlgoForge aims to bridge that gap by connecting **actual C++ execution with visual, step-by-step learning**.

It also demonstrates how a compiled backend, HTTP API, JSON communication, and browser-based interface can work together in one application.

**AlgoForge — See the logic. Follow every step. Understand the algorithm.**
