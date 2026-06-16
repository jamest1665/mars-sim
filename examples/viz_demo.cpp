/**
 * @file viz_demo.cpp
 * @brief Demo of Visualization & Scenario tools on the full stack.
 *        Runs a Monte Carlo and exports results.
 */

#include "mars/viz/viz.hpp"

#include <iostream>

int main() {
    std::cout << "=== Mars Simulator v0.6 — Viz & Scenarios Demo ===\n";
    std::cout << "Running Monte Carlo (50 runs, 3-year missions)...\n\n";

    mars::viz::ScenarioRunner runner;

    auto results = runner.run_monte_carlo(50, 3.0);

    int successes = 0;
    for (const auto& r : results) {
        if (r.mission_successful) successes++;
    }

    std::cout << "Success rate: " << (100.0 * successes / results.size()) << "%\n";

    // Export one detailed run
    auto single = runner.run_single_scenario(3.0, 1.0);
    runner.export_to_csv(single, "single_mission.csv");

    // Export summary
    runner.export_monte_carlo_summary(results, "monte_carlo_summary.csv");

    std::cout << "\nExported:\n";
    std::cout << "  single_mission.csv     (detailed time series)\n";
    std::cout << "  monte_carlo_summary.csv (aggregate statistics)\n";
    std::cout << "\nOpen the CSVs with pandas/matplotlib for plots and analysis.\n";

    return 0;
}
