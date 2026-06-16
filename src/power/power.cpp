/**
 * @file power.cpp
 * @brief Implementation of MarsPower system.
 */

#include "mars/power/power.hpp"

#include <algorithm>
#include <cmath>

namespace mars::power {

MarsPower::MarsPower(PowerConfig cfg) : m_cfg(std::move(cfg)) {
    m_state.battery_capacity_kwh = m_cfg.battery_capacity_kwh;
}

void MarsPower::update(const mars::foundation::EnvironmentState& env_state,
                       double years_elapsed,
                       double demand_kw) {

    m_state.total_demand_kw = demand_kw;

    // Dust accumulation over time
    m_state.dust_on_panels = std::min(0.6, DUST_ACCUMULATION_RATE * years_elapsed);

    // Solar generation (affected by dust, atmosphere, and weather)
    double solar_kw = calculate_solar_generation(env_state);
    m_state.solar_generation_kw = solar_kw;

    // Nuclear (constant if enabled)
    m_state.nuclear_generation_kw = m_cfg.has_nuclear ? m_cfg.nuclear_power_kw : 0.0;

    double total_generation = m_state.solar_generation_kw + m_state.nuclear_generation_kw;

    // Simple battery logic
    double net_power = total_generation - demand_kw;
    double battery_change = net_power * BATTERY_EFFICIENCY * 0.5; // half hour timestep assumption

    m_state.battery_soc = std::clamp(m_state.battery_soc + battery_change / m_state.battery_capacity_kwh, 0.0, 1.0);

    m_state.total_available_kw = total_generation + (m_state.battery_soc * 10.0); // simplistic discharge rate
    m_state.power_critical = (m_state.total_available_kw < demand_kw * 0.9);
}

double MarsPower::calculate_solar_generation(const mars::foundation::EnvironmentState& state) const {
    double base = MARS_SOLAR_CONSTANT * m_cfg.solar_array_area_m2 * 0.25; // rough efficiency
    double dust_factor = 1.0 - m_state.dust_on_panels;
    double atm_factor = std::max(0.5, 1.0 - (state.atmosphere.pressure_pa - 610.0) / 40000.0 * 0.2);
    double weather_factor = std::max(0.3, 1.0 - state.solar.optical_depth_tau * 0.15);

    return base * dust_factor * atm_factor * weather_factor / 1000.0; // to kW
}

void MarsPower::set_config(const PowerConfig& cfg) noexcept {
    m_cfg = cfg;
    m_state.battery_capacity_kwh = cfg.battery_capacity_kwh;
}

} // namespace mars::power
