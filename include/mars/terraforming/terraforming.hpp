#pragma once

/**
 * @file terraforming.hpp
 * @brief MarsTerraforming module: parametric atmosphere thickening, temperature
 *        response, and radiation shielding improvement.
 *
 * Designed to compose with foundation + weather modules via EnvironmentState.
 * Production-grade: noexcept, documented assumptions, extensible Config.
 *
 * Typical usage:
 *   MarsEnvironment env;
 *   MarsWeather weather;
 *   MarsTerraforming terra;
 *   auto state = env.sample_state(loc, ls);
 *   weather.apply_to_state(state, ls, loc);
 *   terra.apply_to_state(state, years_elapsed, loc);
 *   // state now reflects terraformed conditions
 */

#include "constants.hpp"
#include "types.hpp"

#include <mars/foundation/types.hpp>
#include <string_view>

namespace mars::terraforming {

class MarsTerraforming {
public:
    explicit MarsTerraforming(TerraformConfig cfg = {});
    ~MarsTerraforming() = default;

    MarsTerraforming(const MarsTerraforming&) = default;
    MarsTerraforming& operator=(const MarsTerraforming&) = default;

    /// Apply terraforming effects to an EnvironmentState over simulated time
    void apply_to_state(mars::foundation::EnvironmentState& state,
                        double years_elapsed,
                        const mars::foundation::LatLonAlt& loc) const noexcept;

    /// Query terraformed state at a given time (standalone)
    [[nodiscard]] TerraformState sample_state(double years_elapsed,
                                              double current_pressure_pa = CURRENT_PRESSURE_PA) const noexcept;

    /// Simple radiation dose reduction factor from increased column depth
    [[nodiscard]] double radiation_reduction_factor(double pressure_pa) const noexcept;

    void set_config(const TerraformConfig& cfg) noexcept { m_cfg = cfg; }
    [[nodiscard]] const TerraformConfig& config() const noexcept { return m_cfg; }

    [[nodiscard]] std::string_view model_description() const noexcept {
        return "Mars terraforming v0.3: parametric atm thickening + greenhouse + radiation shielding";
    }

private:
    TerraformConfig m_cfg;

    [[nodiscard]] double pressure_after_years(double years) const noexcept;
    [[nodiscard]] double temperature_response(double pressure_pa) const noexcept;
};

} // namespace mars::terraforming
