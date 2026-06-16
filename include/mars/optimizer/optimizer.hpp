#pragma once

/**
 * @file optimizer.hpp
 * @brief Simple optimization layer for mission parameters.
 */

#include <mars/campaign/campaign.hpp>
#include <vector>
#include <string>

namespace mars::optimizer {

struct OptimizationResult {
    agents::AgentStrategy best_strategy;
    double best_supply_interval;
    double best_risk_score;
    double best_sustainability_years;
    std::string recommendation;
};

class MissionOptimizer {
public:
    MissionOptimizer() = default;

    OptimizationResult optimize(double duration_years = 12.0,
                                int initial_population = 6);
};

} // namespace mars::optimizer
