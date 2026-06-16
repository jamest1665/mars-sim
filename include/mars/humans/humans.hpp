#pragma once

/**
 * @file humans.hpp
 * @brief MarsHumans module: radiation health, low-g physiology, nutrition,
 *        and psychological factors based on the habitat + environment state.
 */

#include "constants.hpp"
#include "types.hpp"

#include <mars/foundation/types.hpp>
#include <string_view>

namespace mars::humans {

class MarsHumans {
public:
    explicit MarsHumans(HumanConfig cfg = {});
    ~MarsHumans() = default;

    MarsHumans(const MarsHumans&) = default;
    MarsHumans& operator=(const MarsHumans&) = default;

    /// Update human health state based on environment, habitats, and time
    [[nodiscard]] HumanHealthState update_health(
        const mars::foundation::EnvironmentState& env_state,
        const mars::habitats::ResourceState& resources,
        double years_elapsed) const noexcept;

    void set_config(const HumanConfig& cfg) noexcept { m_cfg = cfg; }
    [[nodiscard]] const HumanConfig& config() const noexcept { return m_cfg; }

    [[nodiscard]] std::string_view model_description() const noexcept {
        return "Mars humans v0.5: radiation health, low-g physiology, nutrition, psychology";
    }

private:
    HumanConfig m_cfg;

    [[nodiscard]] double radiation_health_impact(double dose_sv) const noexcept;
    [[nodiscard]] double low_g_physiology_impact(double years, double exercise) const noexcept;
};

} // namespace mars::humans
