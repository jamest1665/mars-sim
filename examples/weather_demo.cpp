/**
 * @file weather_demo.cpp
 * @brief Demo of Mars weather/dust module composed with foundation.
 *        Shows seasonal dust optical depth, storm events, and impact on solar.
 *
 * Build: cmake --build . --target weather_demo
 * Run: ./weather_demo
 */

#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"

#include <iomanip>
#include <iostream>

using namespace mars::foundation;
using namespace mars::weather;

int main() {
    std::cout << "=== Mars Simulator v0.2 — Weather/Dust Module Demo ===\n";
    std::cout << "Parametric seasonal dust + storm model (MGS/MRO calibrated)\n\n";

    MarsEnvironment env;
    MarsWeather weather;  // default config

    std::cout << "Weather model: " << weather.model_description() << "\n\n";

    // Sample over a Mars year at Gale crater (Curiosity site)
    LatLonAlt gale{-4.5, 137.4, -4500.0};
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Gale Crater dust & solar over Ls (selected seasons):\n";
    std::cout << "Ls    | tau   | Storm? | Global Solar (W/m²) | Notes\n";
    std::cout << "------|-------|--------|---------------------|--------------------\n";

    for (double ls : {0.0, 90.0, 180.0, 225.0, 270.0, 315.0, 360.0}) {
        auto state = env.sample_state(gale, ls, 0.5);           // noon
        weather.apply_to_state(state, ls, gale);

        bool storm = weather.is_storm_active(ls, gale.latitude_deg);
        std::string note = (ls >= 200 && ls <= 320) ? "storm season" : "clear season";
        if (storm) note += " (active)";

        std::cout << std::setw(5) << ls << " | "
                  << std::setw(5) << state.solar.optical_depth_tau << " | "
                  << (storm ? "  YES  " : "   no  ") << " | "
                  << std::setw(19) << state.solar.global_horizontal_wm2 << " | "
                  << note << "\n";
    }

    std::cout << "\nKey observations:\n";
    std::cout << "- τ jumps dramatically in southern summer (Ls 180-360)\n";
    std::cout << "- Global solar on surface drops sharply during storms\n";
    std::cout << "- Foundation radiation/solar now automatically sees dust loading\n";
    std::cout << "\nNext: integrate into habitats (power), terraforming (atm heating), human models.\n";

    return 0;
}
