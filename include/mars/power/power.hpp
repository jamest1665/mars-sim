#pragma once

/**
 * @file power.hpp
 * @brief MarsPower module - realistic solar + nuclear + storage modeling
 *        with dust accumulation and battery dynamics.
 */

#include "constants.hpp"
#include "types.hpp"

#include <mars/foundation/types.hpp>
#include <string_view>

namespace mars::power {

class MarsPower {
public:
    explicit MarsPower(PowerConfig cfg = {});
    ~MarsPower() = default;

    /// Update power state based on environment and time
    void update(const mars::foundation::EnvironmentState& env_state,
                double years_elapsed,
                double demand_kw);

    [[nodiscard]] const PowerState& state() const noexcept { return m_state; }

    void set_config(const PowerConfig& cfg) noexcept;
    [[nodiscard]] const PowerConfig& config() const noexcept { return m_cfg; }

    [[nodiscard]] std::string_view model_description() const noexcept {
        return "Mars power v0.8: solar + nuclear + battery with dust accumulation";
    }

private:
    PowerConfig m_cfg;
    PowerState m_state;

    double calculate_solar_generation(const mars::foundation::EnvironmentState& state) const;
};

} // namespace mars::power
