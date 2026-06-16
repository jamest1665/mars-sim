/**
 * @file foundation_demo.cpp
 * @brief Standalone demo of Mars foundation module.
 *        Builds with: cmake --build . --target foundation_demo
 *        Run: ./foundation_demo
 *
 * Shows: gravity, atm profiles, radiation, solar at sample locations/scenarios.
 * Production use: link lib, query in your sim loop or agent.
 */

#include "mars/foundation/environment.hpp"

#include <iomanip>
#include <iostream>

using namespace mars::foundation;

int main() {
    std::cout << "=== Mars Simulator Foundation v0.1 Demo ===\n";
    std::cout << "NASA-calibrated constants + hydrostatic + parametric models\n\n";

    MarsEnvironment env; // default nominal config

    std::cout << "Model: " << env.model_description() << "\n";
    std::cout << "Gravity: " << env.gravity() << " m/s² (user baseline 3.71)\n\n";

    // Sample locations
    LatLonAlt curiosity{ -4.5, 137.4, -4500.0 }; // Gale crater approx
    LatLonAlt olympus{ 18.65, 226.2, 21900.0 };  // Olympus Mons top
    LatLonAlt hellas{ -42.0, 70.0, -7200.0 };    // Hellas basin floor

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "--- Atmosphere profiles ---\n";
    for (const auto& loc : {curiosity, olympus, hellas}) {
        auto atm = env.atmosphere_at(loc.altitude_m);
        std::cout << "Lat " << loc.latitude_deg << " alt " << loc.altitude_m << "m: "
                  << "P=" << atm.pressure_pa << " Pa, T=" << atm.temperature_k << " K, rho="
                  << atm.density_kgm3 << " kg/m3\n";
    }

    std::cout << "\n--- Radiation (GCR baseline, pressure adjusted) ---\n";
    for (const auto& loc : {curiosity, olympus}) {
        auto rad = env.radiation_at(loc);
        std::cout << "Alt " << loc.altitude_m << "m: GCR equiv " << rad.gcr_dose_equiv_msv_per_day
                  << " mSv/day (total " << rad.total_dose_equiv_msv_per_day << ")\n";
    }

    // SEP example
    auto rad_sep = env.radiation_at(curiosity, -1.0, true);
    std::cout << "Curiosity + SEP event: total dose equiv " << rad_sep.total_dose_equiv_msv_per_day << " mSv/day\n";

    std::cout << "\n--- Solar insolation (Ls=90 northern summer, local noon) ---\n";
    double ls = 90.0;
    double t_noon = 0.5;
    for (const auto& loc : {curiosity, olympus, hellas}) {
        auto sol = env.solar_at(loc, ls, t_noon);
        std::cout << "Lat " << loc.latitude_deg << " alt " << loc.altitude_m << "m: TOA="
                  << sol.toa_irradiance_wm2 << " W/m2, global horiz=" << sol.global_horizontal_wm2
                  << " (tau=" << sol.optical_depth_tau << ", zenith=" << sol.zenith_angle_deg << °)\n";
    }

    // Full state sample
    std::cout << "\n--- Full EnvironmentState sample (Gale, Ls=0, midnight) ---\n";
    auto state = env.sample_state(curiosity, 0.0, 0.0);
    std::cout << "P=" << state.atmosphere.pressure_pa << " Pa | GCR=" << state.radiation.gcr_dose_equiv_msv_per_day
              << " mSv/d | Solar global=" << state.solar.global_horizontal_wm2 << " W/m2 | g="
              << state.gravity_mps2 << "\n";

    std::cout << "\nDemo complete. Extend via MarsEnvironment::Config or subclass for weather/terraform.\n";
    std::cout << "Next: integrate with step 2 Weather/Dust module.\n";
    return 0;
}
