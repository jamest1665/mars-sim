/**
 * @file habitats.cpp
 * @brief Production implementation of MarsHabitats resource loops and sizing.
 *        Environment-aware: better terraformed conditions = lower mass and power needs.
 */

#include "mars/habitats/habitats.hpp"

#include <algorithm>
#include <cmath>

namespace mars::habitats {

MarsHabitats::MarsHabitats(HabitatConfig cfg) : m_cfg(std::move(cfg)) {}

ResourceState MarsHabitats::calculate_resources(
    const mars::foundation::EnvironmentState& env_state,
    double years_terraformed) const noexcept {

    ResourceState res;
    const int crew = m_cfg.crew_size;

    // O2 from atmospheric CO2 (Sabatier + electrolysis) - improves with pressure
    res.o2_production_kg_per_day = calculate_o2_production(env_state.atmosphere.pressure_pa, crew);

    // Water from regolith or recycled - better with higher temp/pressure (easier extraction)
    const double regolith_access = std::min(1.0, years_terraformed / 30.0); // improves over time
    res.water_production_kg_per_day = calculate_water_production(regolith_access, crew);

    // Power: solar degraded by dust/atm, plus optional nuclear
    res.power_generation_kw = calculate_power_generation(env_state);
    if (m_cfg.use_nuclear_power) {
        res.power_generation_kw += 50.0; // example 50 kW reactor
    }

    // Power demand (life support + ISRU)
    const double base_demand = crew * 2.5; // kW rough
    const double isru_demand = (res.o2_production_kg_per_day / 0.5); // rough scaling
    res.power_demand_kw = base_demand + isru_demand;

    // Habitat mass (structure + shielding) - lower radiation = less shielding needed
    res.habitat_mass_kg = calculate_habitat_mass(env_state, crew);
    res.shielding_mass_kg = res.habitat_mass_kg * 0.3; // rough split

    // Sustainability check
    const double o2_balance = res.o2_production_kg_per_day - (crew * O2_CONSUMPTION_KG_PER_PERSON_DAY);
    const double water_balance = res.water_production_kg_per_day - (crew * WATER_CONSUMPTION_KG_PER_PERSON_DAY * (1.0 - m_cfg.eclss_recycle_efficiency));
    res.closed_loop_sustainable = (o2_balance > -0.1) && (water_balance > -0.5) && (res.power_generation_kw > res.power_demand_kw);

    return res;
}

void MarsHabitats::apply_to_state(mars::foundation::EnvironmentState& /*state*/,
                                  const ResourceState& /*resources*/) const noexcept {
    // Placeholder: could add power draw notes or habitat radiation protection back into state
    // For now, the calculation is the primary output
}

double MarsHabitats::calculate_o2_production(double co2_pressure_pa, int crew) const noexcept {
    // Higher pressure = more CO2 available for Sabatier
    const double pressure_factor = std::max(0.3, co2_pressure_pa / 610.0);
    const double base_o2 = crew * O2_CONSUMPTION_KG_PER_PERSON_DAY * 1.1; // slight overproduction
    return base_o2 * pressure_factor * 0.85; // efficiency
}

double MarsHabitats::calculate_water_production(double regolith_access, int crew) const noexcept {
    const double recycled = crew * WATER_CONSUMPTION_KG_PER_PERSON_DAY * m_cfg.eclss_recycle_efficiency;
    const double extracted = crew * 2.0 * regolith_access * REGOLITH_WATER_YIELD * 1000; // scaled
    return recycled + extracted * 0.3; // conservative
}

double MarsHabitats::calculate_power_generation(const mars::foundation::EnvironmentState& state) const noexcept {
    // Solar degraded by optical depth and thicker atmosphere
    const double dust_factor = std::max(0.2, 1.0 - (state.solar.optical_depth_tau - 0.4) * 0.15);
    const double atm_factor = std::max(0.6, 1.0 - (state.atmosphere.pressure_pa - 610.0) / 50000.0 * 0.2);
    const double solar_kw_per_crew = 15.0 * MARS_SOLAR_POWER_FACTOR * dust_factor * atm_factor;
    return solar_kw_per_crew * m_cfg.crew_size * m_cfg.solar_array_efficiency / 0.25;
}

double MarsHabitats::calculate_habitat_mass(const mars::foundation::EnvironmentState& state, int crew) const noexcept {
    double mass = crew * BASE_HABITAT_MASS_PER_PERSON_KG;

    // Radiation shielding reduction with better terraforming
    const double rad_dose = state.radiation.gcr_dose_equiv_msv_per_day * 365.25;
    const double extra_shielding_needed = std::max(0.0, (rad_dose - 300.0) / 100.0); // mSv/year target rough
    mass += extra_shielding_needed * SHIELDING_MASS_KG_PER_M2_PER_SV * 200.0; // area proxy

    // Thicker atmosphere allows lighter structures (less pressure differential)
    const double pressure_relief = std::max(0.7, 1.0 - (state.atmosphere.pressure_pa - 610.0) / 30000.0 * 0.3);
    mass *= pressure_relief;

    return mass;
}

} // namespace mars::habitats
