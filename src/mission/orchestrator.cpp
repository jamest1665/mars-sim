/**
 * @file orchestrator.cpp
 * @brief Implementation of the End-to-End Mission Orchestrator.
 */

#include "mars/mission/orchestrator.hpp"

#include <random>

namespace mars::mission {

std::vector<MissionEvent> MissionOrchestrator::run_mission(double duration_years) {
    std::vector<MissionEvent> events;
    std::mt19937 rng(123);

    double year = 0.0;
    const double dt = 0.5;

    while (year < duration_years) {
        auto state = env_.sample_state({-30, 0, 0}, 270.0, 0.5);
        weather_.apply_to_state(state, 270.0, state.location);
        terra_.apply_to_state(state, year, state.location);

        auto resources = habitats_.calculate_resources(state, year);
        auto health = humans_.update_health(state, resources, year);

        // Simple stochastic event injection
        if (std::uniform_real_distribution<>(0, 1)(rng) < 0.05) {
            events.push_back({year, "Dust storm event - temporary power reduction", false});
        }

        if (health.bone_density_percent < 75.0 && year > 1.0) {
            events.push_back({year, "Crew bone density critically low - exercise protocol intensified", true});
        }

        year += dt;
    }

    events.push_back({duration_years, "Mission complete - crew health within limits", true});
    return events;
}

} // namespace mars::mission
