#ifndef ALGO_FORGE_SEARCHING_H
#define ALGO_FORGE_SEARCHING_H

#include <vector>
#include "../AlgorithmResult.h"
#include "../StepRecorder.h"

namespace algo {
namespace searching {

AlgorithmResult linearSearch(const std::vector<int>& arr, int target);
AlgorithmResult binarySearch(const std::vector<int>& arr, int target);

} // namespace searching
} // namespace algo

#endif // ALGO_FORGE_SEARCHING_H
