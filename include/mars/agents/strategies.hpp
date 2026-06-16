#pragma once

/**
 * @file strategies.hpp
 * @brief Different agent behavior strategies for closed-loop control.
 */

#include <mars/power/power.hpp>
#include <mars/habitats/habitats.hpp>

namespace mars::agents {

enum class AgentStrategy {
    CONSERVATIVE,   // Prioritize safety and resource reserves
    AGGRESSIVE,     // Maximize production and operations
    BALANCED        // Default balanced approach
};

class StrategyAgent {
public:
    explicit StrategyAgent(AgentStrategy strategy = AgentStrategy::BALANCED);

    void step(const mars::power::PowerState& power,
              const mars::habitats::ResourceState& resources,
              double years_elapsed);

    [[nodiscard]] double get_isru_priority() const noexcept { return m_isru_priority; }
    [[nodiscard]] double get_power_allocation() const noexcept { return m_power_allocation; }
    [[nodiscard]] AgentStrategy get_strategy() const noexcept { return m_strategy; }

private:
    AgentStrategy m_strategy;
    double m_isru_priority{0.5};
    double m_power_allocation{1.0};
};

} // namespace mars::agents
