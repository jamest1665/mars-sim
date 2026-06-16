/**
 * @file terraforming.cpp
 * @brief Production implementation of MarsTerraforming.
 *        Assumptions clearly documented. Simple but physically motivated
 *        parametric models suitable for scenario exploration and agent training.
 */

#include "mars/terraforming/terraforming.hpp"

#include <algorithm>
#include <cmath>

namespace mars::terraforming {

MarsTerraforming::MarsTerraforming(TerraformConfig cfg) : m_cfg(std::move(cfg)) {}

void MarsTerraforming::apply_to_state(mars::foundation::EnvironmentState& state,
                                      double years_elapsed,
                                      const mars::foundation::LatLonAlt& /*loc*/) const noexcept {
    const auto tstate = sample_state(years_elapsed, state.atmosphere.pressure_pa);

    // Update atmosphere
    state.atmosphere.pressure_pa = tstate.pressure_pa;
    state.atmosphere.temperature_k = tstate.temperature_k;

    // Radiation shielding improvement
    const double rad_factor = radiation_reduction_factor(tstate.pressure_pa);
    state.radiation.gcr_dose_equiv_msv_per_day *= rad_factor;
    state.radiation.total_dose_equiv_msv_per_day *= rad_factor;

    // Note: solar optical depth can also be affected by thicker atm (minor scattering),
    // but primary effect is already captured via pressure in foundation radiation model.
}

TerraformState MarsTerraforming::sample_state(double years_elapsed,
                                              double current_pressure_pa) const noexcept {
    TerraformState ts;
    ts.pressure_pa = pressure_after_years(years_elapsed);

    // Blend from current toward target
    const double progress = std::min(1.0, years_elapsed / m_cfg.simulation_years);
    ts.pressure_pa = current_pressure_pa + (ts.pressure_pa - current_pressure_pa) * progress;
    ts.pressure_pa = std::clamp(ts.pressure_pa, CURRENT_PRESSURE_PA, MAX_CREDIBLE_PRESSURE_PA);

    ts.temperature_k = temperature_response(ts.pressure_pa);
    ts.radiation_reduction_factor = radiation_reduction_factor(ts.pressure_pa);
    ts.greenhouse_delta_k = ts.temperature_k - 210.0;

    // Rough cumulative ISRU energy proxy (very high-level)
    const double mass_released_kg = (ts.pressure_pa - CURRENT_PRESSURE_PA) * 1e10; // rough Mars area scaling
    ts.isru_energy_cost_kwh = mass_released_kg * ISRU_ENERGY_KWH_PER_KG_CO2;

    return ts;
}

double MarsTerraforming::radiation_reduction_factor(double pressure_pa) const noexcept {
    if (pressure_pa <= CURRENT_PRESSURE_PA) return 1.0;
    const double decades = std::log10(pressure_pa / CURRENT_PRESSURE_PA);
    const double reduction = decades * RADIATION_SHIELDING_PER_DECADE;
    return std::max(0.3, 1.0 - reduction); // floor at 70% reduction for realism
}

double MarsTerraforming::pressure_after_years(double years) const noexcept {
    // Simple exponential approach toward target (engineering ramp)
    const double target = m_cfg.target_pressure_pa;
    const double tau = m_cfg.simulation_years / 3.0; // time constant
    const double achieved = target * (1.0 - std::exp(-years / tau));
    return CURRENT_PRESSURE_PA + achieved;
}

double MarsTerraforming::temperature_response(double pressure_pa) const noexcept {
    if (pressure_pa <= CURRENT_PRESSURE_PA) return 210.0;

    const double pressure_ratio = pressure_pa / CURRENT_PRESSURE_PA;
    const double doublings = std::log2(pressure_ratio);
    const double delta = doublings * GREENHOUSE_SENSITIVITY_K * m_cfg.greenhouse_efficiency;

    // Add baseline warming from thicker atm (simple)
    return 210.0 + delta + (pressure_pa - CURRENT_PRESSURE_PA) * 0.0008;
}

} // namespace mars::terraforming
