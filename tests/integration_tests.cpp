/**
 * @file integration_tests.cpp
 * @brief Basic integration tests for the full Mars Simulator stack (v0.6+).
 *        Run with: ./foundation_tests (extended) or build with Catch2 later.
 */

#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"
#include "mars/terraforming/terraforming.hpp"
#include "mars/habitats/habitats.hpp"
#include "mars/humans/humans.hpp"
#include "mars/viz/viz.hpp"

#include <cassert>
#include <iostream>

using namespace mars::foundation;
using namespace mars::weather;
using namespace mars::terraforming;
using namespace mars::habitats;
using namespace mars::humans;
using namespace mars::viz;

int main() {
    std::cout << "Running integration tests...\n";

    // Test full stack composition
    MarsEnvironment env;
    MarsWeather weather;
    MarsTerraforming terra;
    MarsHabitats habitats;
    MarsHumans humans;

    auto state = env.sample_state({-30, 0, 0}, 270.0, 0.5);
    weather.apply_to_state(state, 270.0, state.location);
    terra.apply_to_state(state, 50.0, state.location);

    auto resources = habitats.calculate_resources(state, 50.0);
    auto health = humans.update_health(state, resources, 3.0);

    assert(health.bone_density_percent > 70.0);
    assert(resources.power_generation_kw > 0);

    // Test viz
    ScenarioRunner runner;
    auto result = runner.run_single_scenario(2.0, 1.0);
    assert(!result.time_series.empty());

    std::cout << "All integration tests PASSED.\n";
    return 0;
}
