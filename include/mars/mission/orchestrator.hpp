#pragma once

/**
 * @file orchestrator.hpp
 * @brief End-to-End Mission Orchestrator with agent control and full stack integration.
 */

#include <mars/foundation/environment.hpp>
#include <mars/weather/weather.hpp>
#include <mars/terraforming/terraforming.hpp>
#include <mars/habitats/habitats.hpp>
#include <mars/humans/humans.hpp>
#include <mars/power/power.hpp>
#include <mars/agents/controller.hpp>
#include <mars/operations/operations.hpp>

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

    /// Run a full end-to-end mission simulation with optional agent control
    std::vector<MissionEvent> run_mission(double duration_years = 3.0, bool use_agent = true);

private:
    mars::foundation::MarsEnvironment env_;
    mars::weather::MarsWeather weather_;
    mars::terraforming::MarsTerraforming terra_;
    mars::habitats::MarsHabitats habitats_;
    mars::humans::MarsHumans humans_;
    mars::power::MarsPower power_;
    mars::agents::AgentController agent_;
    mars::operations::SurfaceOperations ops_;
};

} // namespace mars::mission
