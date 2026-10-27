#ifndef ALGO_FORGE_SORTING_H
#define ALGO_FORGE_SORTING_H

#include <vector>
#include "../AlgorithmResult.h"
#include "../StepRecorder.h"

namespace algo {
namespace sorting {

AlgorithmResult bubbleSort(std::vector<int> arr);
AlgorithmResult selectionSort(std::vector<int> arr);
AlgorithmResult insertionSort(std::vector<int> arr);
AlgorithmResult mergeSort(std::vector<int> arr);
AlgorithmResult quickSort(std::vector<int> arr);

} // namespace sorting
} // namespace algo

#endif // ALGO_FORGE_SORTING_H
