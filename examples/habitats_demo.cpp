/**
 * @file habitats_demo.cpp
 * @brief Demo of Habitats + Resource Loops on top of the full previous stack.
 *        Shows how terraformed conditions enable sustainable closed-loop habitats.
 */

#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"
#include "mars/terraforming/terraforming.hpp"
#include "mars/habitats/habitats.hpp"

#include <iomanip>
#include <iostream>

using namespace mars::foundation;
using namespace mars::weather;
using namespace mars::terraforming;
using namespace mars::habitats;

int main() {
    std::cout << "=== Mars Simulator v0.4 — Habitats + Resource Loops Demo ===\n";
    std::cout << "Full stack: Foundation + Weather + Terraforming + Habitats\n\n";

    MarsEnvironment env;
    MarsWeather weather;
    MarsTerraforming terra;
    MarsHabitats habitats; // default 6 crew

    LatLonAlt site{-30.0, 0.0, 0.0};

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Year | Pressure(kPa) | GCR(mSv/d) | O2 prod(kg/d) | Water prod(kg/d) | Power gen(kW) | Habitat mass(t) | Sustainable?\n";
    std::cout << "-----|---------------|------------|---------------|------------------|---------------|-----------------|-------------\n";

    for (double year : {0.0, 20.0, 50.0, 100.0}) {
        auto state = env.sample_state(site, 270.0, 0.5);
        weather.apply_to_state(state, 270.0, site);
        terra.apply_to_state(state, year, site);

        auto resources = habitats.calculate_resources(state, year);

        std::cout << std::setw(4) << year << " | "
                  << std::setw(13) << (state.atmosphere.pressure_pa / 1000.0) << " | "
                  << std::setw(10) << state.radiation.gcr_dose_equiv_msv_per_day << " | "
                  << std::setw(13) << resources.o2_production_kg_per_day << " | "
                  << std::setw(16) << resources.water_production_kg_per_day << " | "
                  << std::setw(13) << resources.power_generation_kw << " | "
                  << std::setw(15) << (resources.habitat_mass_kg / 1000.0) << " | "
                  << (resources.closed_loop_sustainable ? "   YES   " : "    no   ") << "\n";
    }

    std::cout << "\nKey insight: As terraforming progresses, habitat mass drops and closed-loop sustainability becomes achievable.\n";
    std::cout << "Next: Human survival models (radiation health, low-g physiology, nutrition, psychology).\n";

    return 0;
}
