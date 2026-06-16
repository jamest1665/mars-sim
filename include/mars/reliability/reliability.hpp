#pragma once

/**
 * @file reliability.hpp
 * @brief Probabilistic reliability, failure modes, and maintenance modeling.
 */

#include <mars/foundation/types.hpp>
#include <random>
#include <string>
#include <vector>

namespace mars::reliability {

struct FailureEvent {
    double year{0.0};
    std::string component;
    std::string description;
    bool critical{false};
    bool maintenance_performed{false};
};

class ReliabilityModel {
public:
    ReliabilityModel() = default;

    std::vector<FailureEvent> simulate_failures(double years, double crew_size = 6.0);

    void perform_maintenance(double year, const std::string& component);

private:
    std::mt19937 rng_{42};
    double maintenance_factor_{1.0};
};

} // namespace mars::reliability
