#pragma once

/**
 * @file controller.hpp
 * @brief Closed-loop agent controller that can make decisions and
 *        influence power allocation, ISRU priority, and habitat operations.
 */

#include <mars/power/power.hpp>
#include <mars/habitats/habitats.hpp>
#include <string_view>

namespace mars::agents {

class AgentController {
public:
    AgentController() = default;

    /// Run one decision cycle
    void step(const mars::power::PowerState& power,
              const mars::habitats::ResourceState& resources,
              double years_elapsed);

    [[nodiscard]] double get_isru_priority() const noexcept { return m_isru_priority; }
    [[nodiscard]] double get_power_allocation_factor() const noexcept { return m_power_allocation; }

    [[nodiscard]] std::string_view name() const noexcept {
        return "AgentController v0.8";
    }

private:
    double m_isru_priority{0.5};      // 0-1
    double m_power_allocation{1.0};   // multiplier on demand
};

} // namespace mars::agents
