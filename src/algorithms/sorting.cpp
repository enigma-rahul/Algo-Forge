#include "../../include/algorithms/sorting.h"
#include <algorithm>
#include <sstream>

namespace algo {
namespace sorting {

AlgorithmResult bubbleSort(std::vector<int> arr) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "bubble_sort";
    res.input = {{"values", arr}};

    const int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            recorder.recordCompare(j, j + 1, arr[j], arr[j + 1]);
            if (arr[j] > arr[j + 1]) {
                recorder.recordSwap(j, j + 1, arr[j], arr[j + 1]);
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Optimized early exit
    }

    recorder.recordDone(arr, "Bubble Sort completed: array is sorted!");
    res.result = {{"values", arr}};
    res.steps = recorder.getSteps();
    res.comparisons = recorder.getComparisons();
    res.swaps = recorder.getSwaps();
    return res;
}

AlgorithmResult selectionSort(std::vector<int> arr) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "selection_sort";
    res.input = {{"values", arr}};

    const int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            recorder.recordCompare(minIdx, j, arr[minIdx], arr[j]);
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            recorder.recordSwap(i, minIdx, arr[i], arr[minIdx]);
            std::swap(arr[i], arr[minIdx]);
        }
    }

    recorder.recordDone(arr, "Selection Sort completed: array is sorted!");
    res.result = {{"values", arr}};
    res.steps = recorder.getSteps();
    res.comparisons = recorder.getComparisons();
    res.swaps = recorder.getSwaps();
    return res;
}

AlgorithmResult insertionSort(std::vector<int> arr) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "insertion_sort";
    res.input = {{"values", arr}};

    const int n = static_cast<int>(arr.size());
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        
        std::ostringstream pickMsg;
        pickMsg << "Picked key arr[" << i << "] = " << key << " to insert into sorted prefix";
        recorder.recordCheck(i, key, pickMsg.str());

        while (j >= 0) {
            recorder.recordCompare(j, j + 1, arr[j], key);
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                recorder.recordWrite(j + 1, arr[j], "Shifted element right");
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
        recorder.recordWrite(j + 1, key, "Inserted key into sorted position");
    }

    recorder.recordDone(arr, "Insertion Sort completed: array is sorted!");
    res.result = {{"values", arr}};
    res.steps = recorder.getSteps();
    res.comparisons = recorder.getComparisons();
    res.swaps = recorder.getSwaps();
    return res;
}

namespace {
void merge(std::vector<int>& arr, int left, int mid, int right, StepRecorder& recorder) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        recorder.recordCompare(left + i, mid + 1 + j, L[i], R[j]);
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            recorder.recordWrite(k, L[i], "Merged element from left subarray");
            i++;
        } else {
            arr[k] = R[j];
            recorder.recordWrite(k, R[j], "Merged element from right subarray");
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        recorder.recordWrite(k, L[i], "Appended remaining left element");
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        recorder.recordWrite(k, R[j], "Appended remaining right element");
        j++;
        k++;
    }
}

void mergeSortHelper(std::vector<int>& arr, int left, int right, StepRecorder& recorder) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, recorder);
        mergeSortHelper(arr, mid + 1, right, recorder);
        merge(arr, left, mid, right, recorder);
    }
}
} // anonymous namespace

AlgorithmResult mergeSort(std::vector<int> arr) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "merge_sort";
    res.input = {{"values", arr}};

    if (!arr.empty()) {
        mergeSortHelper(arr, 0, static_cast<int>(arr.size() - 1), recorder);
    }

    recorder.recordDone(arr, "Merge Sort completed: array is sorted!");
    res.result = {{"values", arr}};
    res.steps = recorder.getSteps();
    res.comparisons = recorder.getComparisons();
    res.swaps = recorder.getSwaps();
    return res;
}

namespace {
int partition(std::vector<int>& arr, int low, int high, StepRecorder& recorder) {
    int pivot = arr[high];
    recorder.recordPivot(high, pivot);

    int i = low - 1;
    for (int j = low; j < high; ++j) {
        recorder.recordCompare(j, high, arr[j], pivot);
        if (arr[j] < pivot) {
            i++;
            if (i != j) {
                recorder.recordSwap(i, j, arr[i], arr[j]);
                std::swap(arr[i], arr[j]);
            }
        }
    }
    if (i + 1 != high) {
        recorder.recordSwap(i + 1, high, arr[i + 1], arr[high]);
        std::swap(arr[i + 1], arr[high]);
    }
    return i + 1;
}

void quickSortHelper(std::vector<int>& arr, int low, int high, StepRecorder& recorder) {
    if (low < high) {
        int pi = partition(arr, low, high, recorder);
        quickSortHelper(arr, low, pi - 1, recorder);
        quickSortHelper(arr, pi + 1, high, recorder);
    }
}
} // anonymous namespace

AlgorithmResult quickSort(std::vector<int> arr) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "quick_sort";
    res.input = {{"values", arr}};

    if (!arr.empty()) {
        quickSortHelper(arr, 0, static_cast<int>(arr.size() - 1), recorder);
    }

    recorder.recordDone(arr, "Quick Sort completed: array is sorted!");
    res.result = {{"values", arr}};
    res.steps = recorder.getSteps();
    res.comparisons = recorder.getComparisons();
    res.swaps = recorder.getSwaps();
    return res;
}

} // namespace sorting
} // namespace algo
