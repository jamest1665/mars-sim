/**
 * @file controller.cpp
 * @brief Simple but functional closed-loop agent controller.
 */

#include "mars/agents/controller.hpp"

#include <algorithm>

namespace mars::agents {

void AgentController::step(const mars::power::PowerState& power,
                           const mars::habitats::ResourceState& resources,
                           double /*years_elapsed*/) {

    // If power is tight, reduce ISRU priority and power demand
    if (power.power_critical || power.total_available_kw < power.total_demand_kw * 0.85) {
        m_isru_priority = std::max(0.2, m_isru_priority * 0.7);
        m_power_allocation = 0.85;
    } else {
        m_isru_priority = std::min(1.0, m_isru_priority * 1.1);
        m_power_allocation = 1.0;
    }

    // If O2 is low, boost ISRU
    if (resources.o2_production_kg_per_day < 4.0) {
        m_isru_priority = std::min(1.0, m_isru_priority + 0.15);
    }
}

} // namespace mars::agents
