#include "../../include/algorithms/searching.h"
#include <sstream>

namespace algo {
namespace searching {

AlgorithmResult linearSearch(const std::vector<int>& arr, int target) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "linear_search";
    res.input = {{"values", arr}, {"target", target}};

    bool found = false;
    int foundIdx = -1;

    for (size_t i = 0; i < arr.size(); ++i) {
        recorder.recordCheck(static_cast<int>(i), arr[i]);
        if (arr[i] == target) {
            recorder.recordFound(static_cast<int>(i), target);
            found = true;
            foundIdx = static_cast<int>(i);
            break;
        }
    }

    if (!found) {
        recorder.recordNotFound(target);
    }

    res.result = {
        {"found", found},
        {"index", foundIdx},
        {"target", target}
    };
    res.steps = recorder.getSteps();
    res.comparisons = recorder.getComparisons();
    return res;
}

AlgorithmResult binarySearch(const std::vector<int>& arr, int target) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "binary_search";
    res.input = {{"values", arr}, {"target", target}};

    // Requirement: Strict validation that input is sorted
    for (size_t i = 0; i + 1 < arr.size(); ++i) {
        if (arr[i] > arr[i + 1]) {
            res.success = false;
            std::ostringstream err;
            err << "Binary Search requires a sorted array. Element at index " << (i + 1)
                << " (" << arr[i + 1] << ") is smaller than preceding element at index " 
                << i << " (" << arr[i] << ").";
            res.error_message = err.str();
            return res;
        }
    }

    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;
    bool found = false;
    int foundIdx = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        std::ostringstream winMsg;
        winMsg << "Search window: [" << low << " .. " << high << "], mid index is " << mid;
        recorder.recordWindow(low, high, mid, winMsg.str());
        recorder.recordCheck(mid, arr[mid]);

        if (arr[mid] == target) {
            recorder.recordFound(mid, target);
            found = true;
            foundIdx = mid;
            break;
        } else if (arr[mid] < target) {
            std::ostringstream msg;
            msg << "arr[" << mid << "] (" << arr[mid] << ") < " << target << ": narrowing search to right half";
            low = mid + 1;
        } else {
            std::ostringstream msg;
            msg << "arr[" << mid << "] (" << arr[mid] << ") > " << target << ": narrowing search to left half";
            high = mid - 1;
        }
    }

    if (!found) {
        recorder.recordNotFound(target);
    }

    res.result = {
        {"found", found},
        {"index", foundIdx},
        {"target", target}
    };
    res.steps = recorder.getSteps();
    res.comparisons = recorder.getComparisons();
    return res;
}

} // namespace searching
} // namespace algo
