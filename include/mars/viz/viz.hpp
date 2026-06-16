#pragma once

/**
 * @file viz.hpp
 * @brief Visualization and Scenario tools for the Mars Simulator.
 *        Provides data export (CSV/JSON), time-series collection, and
 *        basic Monte Carlo scenario running.
 *
 * Production grade: composes with the full stack (Foundation through Humans).
 */

#include "constants.hpp"
#include "types.hpp"

#include <mars/foundation/environment.hpp>
#include <mars/weather/weather.hpp>
#include <mars/terraforming/terraforming.hpp>
#include <mars/habitats/habitats.hpp>
#include <mars/humans/humans.hpp>

#include <string>
#include <vector>

namespace mars::viz {

class ScenarioRunner {
public:
    ScenarioRunner() = default;

    /// Run a single deterministic scenario and collect time series
    [[nodiscard]] ScenarioResult run_single_scenario(
        double mission_years = 3.0,
        double terraforming_aggressiveness = 1.0) const;

    /// Run Monte Carlo with variation in key parameters (storms, terraforming rate, etc.)
    [[nodiscard]] std::vector<ScenarioResult> run_monte_carlo(
        int num_runs = DEFAULT_MONTE_CARLO_RUNS,
        double mission_years = 3.0) const;

    /// Export a ScenarioResult to CSV (easy to plot with Python/pandas)
    static bool export_to_csv(const ScenarioResult& result, const std::string& filename);

    /// Export multiple results summary (success rate, average final health, etc.)
    static bool export_monte_carlo_summary(const std::vector<ScenarioResult>& results,
                                           const std::string& filename);

    /// Print a nice human-readable mission summary to console
    static void print_mission_report(const ScenarioResult& result);
};

} // namespace mars::viz
