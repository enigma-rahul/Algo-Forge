#include "../include/StepRecorder.h"
#include <sstream>

namespace algo {

StepRecorder::StepRecorder(size_t maxSteps)
    : maxSteps_(maxSteps), comparisons_(0), swaps_(0) {}

void StepRecorder::addStep(Step step) {
    if (steps_.size() >= maxSteps_) return;
    step.step = static_cast<int>(steps_.size() + 1);
    steps_.push_back(std::move(step));
}

void StepRecorder::clear() {
    steps_.clear();
    comparisons_ = 0;
    swaps_ = 0;
}

void StepRecorder::recordCompare(int i, int j, int val_i, int val_j, const std::string& msg) {
    comparisons_++;
    Step s;
    s.type = "compare";
    s.indices = {i, j};
    s.values = {val_i, val_j};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Comparing arr[" << i << "] (" << val_i << ") and arr[" << j << "] (" << val_j << ")";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordSwap(int i, int j, int val_i, int val_j, const std::string& msg) {
    swaps_++;
    Step s;
    s.type = "swap";
    s.indices = {i, j};
    s.values = {val_i, val_j};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Swapping arr[" << i << "] (" << val_j << ") and arr[" << j << "] (" << val_i << ")";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordWrite(int idx, int val, const std::string& msg) {
    Step s;
    s.type = "write";
    s.indices = {idx};
    s.values = {val};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Writing value " << val << " at index " << idx;
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordPivot(int pivotIdx, int pivotVal, const std::string& msg) {
    Step s;
    s.type = "pivot";
    s.indices = {pivotIdx};
    s.values = {pivotVal};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Chosen pivot: arr[" << pivotIdx << "] = " << pivotVal;
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordCheck(int idx, int val, const std::string& msg) {
    Step s;
    s.type = "check";
    s.indices = {idx};
    s.values = {val};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Checking element at index " << idx << " (" << val << ")";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordWindow(int low, int high, int mid, const std::string& msg) {
    Step s;
    s.type = "window";
    s.indices = {low, high, mid};
    s.message = msg.empty() ? "Updated search range" : msg;
    addStep(s);
}

void StepRecorder::recordFound(int idx, int val, const std::string& msg) {
    Step s;
    s.type = "found";
    s.indices = {idx};
    s.values = {val};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Target value " << val << " found at index " << idx << "!";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordNotFound(int target, const std::string& msg) {
    Step s;
    s.type = "not_found";
    s.values = {target};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Target " << target << " was not found in the array.";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordVisit(int node, const std::string& msg) {
    Step s;
    s.type = "visit";
    s.node = node;
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Visiting node " << node;
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordEnqueue(int node, const std::vector<int>& currentQueue, const std::string& msg) {
    Step s;
    s.type = "enqueue";
    s.node = node;
    s.queue = currentQueue;
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Enqueued neighbor node " << node;
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordDequeue(int node, const std::vector<int>& currentQueue, const std::string& msg) {
    Step s;
    s.type = "dequeue";
    s.node = node;
    s.queue = currentQueue;
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Dequeued node " << node << " for exploration";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordPush(int node, const std::vector<int>& currentStack, const std::string& msg) {
    Step s;
    s.type = "push";
    s.node = node;
    s.queue = currentStack;
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Pushed node " << node << " onto call stack";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordPop(int node, const std::vector<int>& currentStack, const std::string& msg) {
    Step s;
    s.type = "pop";
    s.node = node;
    s.queue = currentStack;
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Backtracked / popped node " << node << " from call stack";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordRelax(int u, int v, int weight, int newDist, 
                               const std::map<std::string, int>& dists, const std::string& msg) {
    Step s;
    s.type = "relax";
    s.indices = {u, v, weight};
    s.distances = dists;
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Relaxed edge (" << u << " -> " << v << ", w=" << weight 
            << "), updated dist[" << v << "] = " << newDist;
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordPath(const std::vector<int>& pathNodes, const std::string& msg) {
    Step s;
    s.type = "path";
    s.path = pathNodes;
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Shortest path reconstructed: ";
        for (size_t i = 0; i < pathNodes.size(); ++i) {
            oss << pathNodes[i] << (i + 1 < pathNodes.size() ? " -> " : "");
        }
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordGridOpen(int r, int c, int f, int g, int h, const std::string& msg) {
    Step s;
    s.type = "open";
    s.indices = {r, c};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Added cell (" << r << "," << c << ") to open set [f=" << f << ", g=" << g << ", h=" << h << "]";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordGridClose(int r, int c, const std::string& msg) {
    Step s;
    s.type = "close";
    s.indices = {r, c};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Closed cell (" << r << "," << c << ")";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordGridPath(const std::vector<std::pair<int, int>>& pathCoords, const std::string& msg) {
    Step s;
    s.type = "path";
    // Pack 2D coords as flattened vector [r0, c0, r1, c1, ...]
    for (const auto& pt : pathCoords) {
        s.indices.push_back(pt.first);
        s.indices.push_back(pt.second);
    }
    s.message = msg.empty() ? "A* path calculated successfully!" : msg;
    addStep(s);
}

void StepRecorder::recordTreeInsert(int val, const std::string& msg) {
    Step s;
    s.type = "insert";
    s.values = {val};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "Inserted key " << val << " into BST";
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordTreeSearchStep(int val, const std::string& branch, const std::string& msg) {
    Step s;
    s.type = "search_step";
    s.values = {val};
    if (msg.empty()) {
        std::ostringstream oss;
        oss << "At node " << val << ", branching " << branch;
        s.message = oss.str();
    } else {
        s.message = msg;
    }
    addStep(s);
}

void StepRecorder::recordDone(const std::vector<int>& finalValues, const std::string& msg) {
    Step s;
    s.type = "done";
    s.values = finalValues;
    s.message = msg;
    addStep(s);
}

} // namespace algo
