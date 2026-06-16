#pragma once

/**
 * @file types.hpp
 * @brief Data types for Mars power systems.
 */

namespace mars::power {

struct PowerState {
    double solar_generation_kw{0.0};
    double nuclear_generation_kw{0.0};
    double battery_soc{0.8};           // State of charge (0-1)
    double battery_capacity_kwh{500.0};
    double dust_on_panels{0.15};       // Fraction of panel covered in dust
    double total_available_kw{0.0};
    double total_demand_kw{0.0};
    bool power_critical{false};
};

struct PowerConfig {
    double solar_array_area_m2 = 200.0;
    bool has_nuclear = false;
    double nuclear_power_kw = 40.0;
    double battery_capacity_kwh = 500.0;
};

} // namespace mars::power
