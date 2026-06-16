#pragma once

/**
 * @file types.hpp
 * @brief Data types for Mars terraforming module.
 */

#include "constants.hpp"

namespace mars::terraforming {

/// Snapshot of terraformed atmospheric state
struct TerraformState {
    double pressure_pa{CURRENT_PRESSURE_PA};
    double temperature_k{210.0};           // surface avg
    double radiation_reduction_factor{1.0}; // 1.0 = current, <1.0 = shielded
    double greenhouse_delta_k{0.0};
    double isru_energy_cost_kwh{0.0};      // cumulative proxy
};

/// Configuration for terraforming scenario
struct TerraformConfig {
    double target_pressure_pa = TARGET_HABITABLE_PRESSURE_PA;
    double greenhouse_efficiency = 1.0;    // multiplier for artificial agents
    double isru_rate_kg_per_year = 1e12;   // massive industrial scale
    bool enable_magnetic_shielding = false; // future extension
    double simulation_years = 100.0;       // how far to project
    // Future: specific greenhouse gas mix, orbital mirror area, etc.
};

} // namespace mars::terraforming
