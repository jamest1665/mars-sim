#pragma once

/**
 * @file types.hpp
 * @brief Data types for Mars habitats module.
 */

#include "constants.hpp"

namespace mars::habitats {

struct ResourceState {
    double o2_production_kg_per_day{0.0};
    double water_production_kg_per_day{0.0};
    double power_demand_kw{0.0};
    double power_generation_kw{0.0};
    double habitat_mass_kg{0.0};
    double shielding_mass_kg{0.0};
    bool closed_loop_sustainable{false};
};

struct HabitatConfig {
    int crew_size = DEFAULT_CREW_SIZE;
    double eclss_recycle_efficiency = ECLSS_WATER_RECYCLE_EFFICIENCY;
    bool use_nuclear_power = false;
    double solar_array_efficiency = 0.25;
    double target_radiation_dose_msv_per_year = 50.0; // ambitious
};

} // namespace mars::habitats
