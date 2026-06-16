/**
 * @file terraforming_demo.cpp
 * @brief Production demo of full stack: Foundation + Weather + Terraforming.
 *        Shows multi-decade terraforming scenario at a mid-latitude site.
 */

#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"
#include "mars/terraforming/terraforming.hpp"

#include <iomanip>
#include <iostream>

using namespace mars::foundation;
using namespace mars::weather;
using namespace mars::terraforming;

int main() {
    std::cout << "=== Mars Simulator v0.3 — Full Stack Terraforming Demo ===\n";
    std::cout << "Foundation + Weather + Terraforming over 100 years\n\n";

    MarsEnvironment env;
    MarsWeather weather;
    MarsTerraforming terra;  // default aggressive config

    LatLonAlt site{-30.0, 0.0, 0.0}; // mid-southern latitude, representative site

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Year | Pressure (kPa) | Temp (K) | GCR dose (mSv/d) | Solar global (W/m²) | Notes\n";
    std::cout << "-----|----------------|----------|------------------|---------------------|--------------------\n";

    for (double year : {0.0, 10.0, 25.0, 50.0, 75.0, 100.0}) {
        // Start from foundation at a representative Ls
        auto state = env.sample_state(site, 270.0, 0.5); // southern summer noon
        weather.apply_to_state(state, 270.0, site);
        terra.apply_to_state(state, year, site);

        std::string note = (year < 20) ? "early stage" : (year < 60) ? "active thickening" : "approaching target";

        std::cout << std::setw(4) << year << " | "
                  << std::setw(14) << (state.atmosphere.pressure_pa / 1000.0) << " | "
                  << std::setw(8) << state.atmosphere.temperature_k << " | "
                  << std::setw(16) << state.radiation.gcr_dose_equiv_msv_per_day << " | "
                  << std::setw(19) << state.solar.global_horizontal_wm2 << " | "
                  << note << "\n";
    }

    std::cout << "\nObservations:\n";
    std::cout << "- Pressure and temperature rise over decades\n";
    std::cout << "- GCR dose drops significantly with thicker atmosphere\n";
    std::cout << "- Surface solar improves as dust effects are partially offset by warming\n";
    std::cout << "\nThis state is now suitable as input for habitat and human survival modules.\n";

    return 0;
}
