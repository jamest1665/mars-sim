#pragma once

/**
 * @file habitats.hpp
 * @brief MarsHabitats module: ISRU, ECLSS, power, and habitat sizing
 *        that depends on the terraformed environment state.
 *
 * Production grade: composes with previous modules via EnvironmentState.
 */

#include "constants.hpp"
#include "types.hpp"

#include <mars/foundation/types.hpp>
#include <string_view>

namespace mars::habitats {

class MarsHabitats {
public:
    explicit MarsHabitats(HabitatConfig cfg = {});
    ~MarsHabitats() = default;

    MarsHabitats(const MarsHabitats&) = default;
    MarsHabitats& operator=(const MarsHabitats&) = default;

    /// Calculate resource loops and habitat requirements based on current environment
    [[nodiscard]] ResourceState calculate_resources(
        const mars::foundation::EnvironmentState& env_state,
        double years_terraformed = 50.0) const noexcept;

    /// Apply habitat effects / requirements back to state if needed (e.g. power draw notes)
    void apply_to_state(mars::foundation::EnvironmentState& state,
                        const ResourceState& resources) const noexcept;

    void set_config(const HabitatConfig& cfg) noexcept { m_cfg = cfg; }
    [[nodiscard]] const HabitatConfig& config() const noexcept { return m_cfg; }

    [[nodiscard]] std::string_view model_description() const noexcept {
        return "Mars habitats v0.4: ISRU + ECLSS + power + shielding sized to environment";
    }

private:
    HabitatConfig m_cfg;

    [[nodiscard]] double calculate_o2_production(double co2_pressure_pa, int crew) const noexcept;
    [[nodiscard]] double calculate_water_production(double regolith_access, int crew) const noexcept;
    [[nodiscard]] double calculate_power_generation(const mars::foundation::EnvironmentState& state) const noexcept;
    [[nodiscard]] double calculate_habitat_mass(const mars::foundation::EnvironmentState& state, int crew) const noexcept;
};

} // namespace mars::habitats
