/**
 * @file optimizer.cpp
 * @brief Basic mission parameter optimizer.
 */

#include "mars/optimizer/optimizer.hpp"

#include <algorithm>
#include <limits>

namespace mars::optimizer {

OptimizationResult MissionOptimizer::optimize(double duration_years, int initial_population) {
    OptimizationResult best;
    best.best_risk_score = std::numeric_limits<double>::max();

    std::vector<agents::AgentStrategy> strategies = {
        agents::AgentStrategy::CONSERVATIVE,
        agents::AgentStrategy::BALANCED,
        agents::AgentStrategy::AGGRESSIVE
    };

    std::vector<double> supply_intervals = {1.5, 2.0, 2.5, 3.0};

    for (auto strat : strategies) {
        for (double interval : supply_intervals) {
            campaign::CampaignSimulator sim;
            auto result = sim.run_campaign(duration_years, strat, initial_population, interval);

            if (result.final_risk_score < best.best_risk_score && result.campaign_success) {
                best.best_strategy = strat;
                best.best_supply_interval = interval;
                best.best_risk_score = result.final_risk_score;
                best.best_sustainability_years = result.colony_sustainability_years;

                std::string strat_name = (strat == agents::AgentStrategy::CONSERVATIVE) ? "Conservative" :
                                         (strat == agents::AgentStrategy::AGGRESSIVE) ? "Aggressive" : "Balanced";

                best.recommendation = "Use " + strat_name + " strategy with supply every " +
                                      std::to_string(interval) + " years.";
            }
        }
    }

    if (best.best_risk_score == std::numeric_limits<double>::max()) {
        best.recommendation = "No viable strategy found for given parameters.";
    }

    return best;
}

} // namespace mars::optimizer
