#ifndef ALGO_FORGE_STEP_RECORDER_H
#define ALGO_FORGE_STEP_RECORDER_H

#include <vector>
#include <string>
#include <map>
#include <utility>
#include "Step.h"

namespace algo {

/**
 * Thread-safe / execution-safe recorder of algorithm steps.
 * Provides intuitive, high-level recording methods for all algorithm families.
 */
class StepRecorder {
public:
    explicit StepRecorder(size_t maxSteps = 5000);

    // Array / Sorting Operations
    void recordCompare(int i, int j, int val_i, int val_j, const std::string& msg = "");
    void recordSwap(int i, int j, int val_i, int val_j, const std::string& msg = "");
    void recordWrite(int idx, int val, const std::string& msg = "");
    void recordPivot(int pivotIdx, int pivotVal, const std::string& msg = "");
    
    // Search Operations
    void recordCheck(int idx, int val, const std::string& msg = "");
    void recordWindow(int low, int high, int mid, const std::string& msg = "");
    void recordFound(int idx, int val, const std::string& msg = "");
    void recordNotFound(int target, const std::string& msg = "");

    // Graph Operations
    void recordVisit(int node, const std::string& msg = "");
    void recordEnqueue(int node, const std::vector<int>& currentQueue, const std::string& msg = "");
    void recordDequeue(int node, const std::vector<int>& currentQueue, const std::string& msg = "");
    void recordPush(int node, const std::vector<int>& currentStack, const std::string& msg = "");
    void recordPop(int node, const std::vector<int>& currentStack, const std::string& msg = "");
    void recordRelax(int u, int v, int weight, int newDist, 
                     const std::map<std::string, int>& dists, const std::string& msg = "");
    void recordPath(const std::vector<int>& pathNodes, const std::string& msg = "");

    // Grid (A*) Operations
    void recordGridOpen(int r, int c, int f, int g, int h, const std::string& msg = "");
    void recordGridClose(int r, int c, const std::string& msg = "");
    void recordGridPath(const std::vector<std::pair<int, int>>& pathCoords, const std::string& msg = "");

    // Tree (BST) Operations
    void recordTreeInsert(int val, const std::string& msg = "");
    void recordTreeSearchStep(int val, const std::string& branch, const std::string& msg = "");

    // Termination
    void recordDone(const std::vector<int>& finalValues, const std::string& msg = "Execution complete");

    // Accessors
    const std::vector<Step>& getSteps() const { return steps_; }
    int getComparisons() const { return comparisons_; }
    int getSwaps() const { return swaps_; }
    size_t size() const { return steps_.size(); }
    void clear();

private:
    void addStep(Step step);

    std::vector<Step> steps_;
    size_t maxSteps_;
    int comparisons_ = 0;
    int swaps_ = 0;
};

} // namespace algo

#endif // ALGO_FORGE_STEP_RECORDER_H
