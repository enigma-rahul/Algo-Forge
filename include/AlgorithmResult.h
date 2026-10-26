#ifndef ALGO_FORGE_ALGORITHM_RESULT_H
#define ALGO_FORGE_ALGORITHM_RESULT_H

#include <string>
#include <vector>
#include "Step.h"
#include "../third_party/json.hpp"

namespace algo {

/**
 * Encapsulates the entire output of an algorithm execution:
 * algorithm key, original input, final result, execution metrics, and ordered steps.
 */
struct AlgorithmResult {
    std::string algorithm;
    nlohmann::json input;
    nlohmann::json result;
    
    int comparisons = 0;
    int swaps = 0;
    std::vector<Step> steps;

    bool success = true;
    std::string error_message;

    nlohmann::json to_json() const {
        if (!success) {
            nlohmann::json err;
            err["error"] = "Execution Error";
            err["message"] = error_message;
            return err;
        }

        nlohmann::json j;
        j["algorithm"] = algorithm;
        j["input"] = input;
        j["result"] = result;
        
        nlohmann::json meta;
        meta["step_count"] = steps.size();
        meta["comparisons"] = comparisons;
        meta["swaps"] = swaps;
        j["metadata"] = meta;

        nlohmann::json stepList = nlohmann::json::array();
        for (const auto& step : steps) {
            stepList.push_back(step.to_json());
        }
        j["steps"] = stepList;

        return j;
    }
};

} // namespace algo

#endif // ALGO_FORGE_ALGORITHM_RESULT_H
