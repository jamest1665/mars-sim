/**
 * @file orchestrator.cpp
 * @brief Implementation of the End-to-End Mission Orchestrator with agent control.
 */

#include "mars/mission/orchestrator.hpp"

#include <random>

namespace mars::mission {

std::vector<MissionEvent> MissionOrchestrator::run_mission(double duration_years, bool use_agent) {
    std::vector<MissionEvent> events;
    std::mt19937 rng(123);

    double year = 0.0;
    const double dt = 0.25;
    double crew_size = 6.0;

    while (year < duration_years) {
        auto state = env_.sample_state({-30, 0, 0}, 270.0, 0.5);
        weather_.apply_to_state(state, 270.0, state.location);
        terra_.apply_to_state(state, year, state.location);

        auto resources = habitats_.calculate_resources(state, year);
        auto health = humans_.update_health(state, resources, year);

        // Update power system
        double base_demand = resources.power_demand_kw + 15.0;
        power_.update(state, year, base_demand);

        // Update surface operations
        ops_.update(state, year, crew_size);

        // Run agent decision loop
        if (use_agent) {
            agent_.step(power_.state(), resources, year);
        }

        // Stochastic events
        if (std::uniform_real_distribution<>(0, 1)(rng) < 0.04) {
            events.push_back({year, "Significant dust storm - reduced solar output", false});
        }

        if (health.bone_density_percent < 78.0 && year > 1.5) {
            events.push_back({year, "Crew health alert: intensified countermeasures required", true});
        }

        if (power_.state().power_critical) {
            events.push_back({year, "Power system critical - agent reduced non-essential loads", true});
        }

        year += dt;
    }

    events.push_back({duration_years, "Mission complete", true});
    return events;
}

} // namespace mars::mission
