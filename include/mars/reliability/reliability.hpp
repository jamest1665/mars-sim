#pragma once

/**
 * @file reliability.hpp
 * @brief Basic probabilistic reliability and failure modeling.
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
};

class ReliabilityModel {
public:
    ReliabilityModel() = default;

    /// Simulate possible failures over time
    std::vector<FailureEvent> simulate_failures(double years, double crew_size = 6.0);

private:
    std::mt19937 rng_{42};
};

} // namespace mars::reliability
