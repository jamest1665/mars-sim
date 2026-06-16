/**
 * @file strategies.cpp
 * @brief Implementation of different agent strategies.
 */

#include "mars/agents/strategies.hpp"

#include <algorithm>

namespace mars::agents {

StrategyAgent::StrategyAgent(AgentStrategy strategy) : m_strategy(strategy) {}

void StrategyAgent::step(const mars::power::PowerState& power,
                         const mars::habitats::ResourceState& resources,
                         double /*years_elapsed*/) {

    switch (m_strategy) {
        case AgentStrategy::CONSERVATIVE:
            if (power.battery_soc < 0.4 || resources.o2_production_kg_per_day < 6.0) {
                m_isru_priority = 0.3;
                m_power_allocation = 0.75;
            } else {
                m_isru_priority = 0.6;
                m_power_allocation = 0.9;
            }
            break;

        case AgentStrategy::AGGRESSIVE:
            m_isru_priority = 0.95;
            m_power_allocation = 1.1;
            if (power.power_critical) {
                m_isru_priority = 0.7;
            }
            break;

        case AgentStrategy::BALANCED:
        default:
            if (power.power_critical) {
                m_isru_priority = std::max(0.25, m_isru_priority * 0.6);
                m_power_allocation = 0.85;
            } else {
                m_isru_priority = std::min(0.9, m_isru_priority * 1.05);
                m_power_allocation = 1.0;
            }
            break;
    }
}

} // namespace mars::agents
