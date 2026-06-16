#pragma once

/**
 * @file operations.hpp
 * @brief Basic surface operations and mobility modeling.
 */

#include "constants.hpp"

#include <mars/foundation/types.hpp>

namespace mars::operations {

struct OperationsState {
    double rover_uptime{0.95};
    double eva_hours_per_day{2.0};
    double equipment_dust_damage{0.1};
    double total_power_demand_kw{0.0};
};

class SurfaceOperations {
public:
    SurfaceOperations() = default;

    void update(const mars::foundation::EnvironmentState& env_state,
                double years_elapsed,
                double crew_size);

    [[nodiscard]] const OperationsState& state() const noexcept { return m_state; }

private:
    OperationsState m_state;
};

} // namespace mars::operations
