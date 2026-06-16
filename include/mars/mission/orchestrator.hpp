#pragma once

/**
 * @file orchestrator.hpp
 * @brief End-to-End Mission Orchestrator + Logistics concepts.
 *        Time-stepped simulation tying the full stack with stochastic events.
 */

#include <mars/foundation/environment.hpp>
#include <mars/weather/weather.hpp>
#include <mars/terraforming/terraforming.hpp>
#include <mars/habitats/habitats.hpp>
#include <mars/humans/humans.hpp>

#include <vector>
#include <string>

namespace mars::mission {

struct MissionEvent {
    double year{0.0};
    std::string description;
    bool is_critical{false};
};

class MissionOrchestrator {
public:
    MissionOrchestrator() = default;

    /// Run a full end-to-end mission simulation
    std::vector<MissionEvent> run_mission(double duration_years = 5.0);

private:
    // Internal stack
    mars::foundation::MarsEnvironment env_;
    mars::weather::MarsWeather weather_;
    mars::terraforming::MarsTerraforming terra_;
    mars::habitats::MarsHabitats habitats_;
    mars::humans::MarsHumans humans_;
};

} // namespace mars::mission
