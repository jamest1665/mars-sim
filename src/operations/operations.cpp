/**
 * @file operations.cpp
 * @brief Surface operations implementation.
 */

#include "mars/operations/operations.hpp"

#include <algorithm>

namespace mars::operations {

void SurfaceOperations::update(const mars::foundation::EnvironmentState& /*env_state*/,
                               double years_elapsed,
                               double crew_size) {

    m_state.equipment_dust_damage = std::min(0.5, DUST_DAMAGE_RATE * years_elapsed);
    m_state.rover_uptime = std::max(0.6, 0.95 - m_state.equipment_dust_damage);

    // Simple power demand from operations
    m_state.total_power_demand_kw = ROVER_POWER_KW + (EVA_POWER_KW * crew_size * 0.3);
    m_state.eva_hours_per_day = std::max(0.5, 2.0 - m_state.equipment_dust_damage * 5.0);
}

} // namespace mars::operations
