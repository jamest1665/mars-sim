/**
 * @file strategies.cpp
 * @brief Implementation of adaptive agent strategies.
 */

#include "mars/agents/strategies.hpp"

#include <algorithm>

namespace mars::agents {

StrategyAgent::StrategyAgent(AgentStrategy strategy) : m_strategy(strategy) {}

void StrategyAgent::step(const mars::power::PowerState& power,
                         const mars::habitats::ResourceState& resources,
                         double /*years_elapsed*/) {

    if (power.power_critical || resources.o2_production_kg_per_day < 4.0) {
        m_stress_level = std::min(1.0, m_stress_level + 0.15);
        m_consecutive_critical_periods++;
    } else {
        m_stress_level = std::max(0.0, m_stress_level - 0.08);
        m_consecutive_critical_periods = 0;
    }

    switch (m_strategy) {
        case AgentStrategy::CONSERVATIVE:
            if (m_stress_level > 0.5 || power.battery_soc < 0.35) {
                m_isru_priority = 0.25;
                m_power_allocation = 0.7;
            } else {
                m_isru_priority = 0.55;
                m_power_allocation = 0.9;
            }
            break;

        case AgentStrategy::AGGRESSIVE:
            m_isru_priority = 0.9;
            m_power_allocation = 1.15;
            if (m_consecutive_critical_periods > 2 || power.power_critical) {
                m_isru_priority = 0.65;
            }
            break;

        case AgentStrategy::BALANCED:
        default:
            if (power.power_critical || m_stress_level > 0.6) {
                m_isru_priority = std::max(0.3, m_isru_priority * 0.7);
                m_power_allocation = 0.8;
            } else {
                m_isru_priority = std::min(0.85, m_isru_priority * 1.08);
                m_power_allocation = 1.0;
            }
            break;
    }
}

void StrategyAgent::reset_memory() {
    m_stress_level = 0.0;
    m_consecutive_critical_periods = 0;
}

} // namespace mars::agents
