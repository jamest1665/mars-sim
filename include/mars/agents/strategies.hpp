#pragma once

/**
 * @file strategies.hpp
 * @brief Different agent behavior strategies with adaptive intelligence.
 */

#include <mars/power/power.hpp>
#include <mars/habitats/habitats.hpp>

namespace mars::agents {

enum class AgentStrategy {
    CONSERVATIVE,
    AGGRESSIVE,
    BALANCED
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

    void reset_memory();

private:
    AgentStrategy m_strategy;
    double m_isru_priority{0.5};
    double m_power_allocation{1.0};

    // Adaptive memory
    double m_stress_level{0.0};
    int m_consecutive_critical_periods{0};
};

} // namespace mars::agents
