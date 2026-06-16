/**
 * @file reliability.cpp
 * @brief Simple reliability and failure simulation.
 */

#include "mars/reliability/reliability.hpp"

#include <algorithm>

namespace mars::reliability {

std::vector<FailureEvent> ReliabilityModel::simulate_failures(double years, double /*crew_size*/) {
    std::vector<FailureEvent> events;

    // Simple failure rate model (higher with dust exposure)
    double base_rate = 0.03; // per quarter year

    for (double y = 0.5; y < years; y += 0.5) {
        if (std::uniform_real_distribution<>(0, 1)(rng_) < base_rate) {
            events.push_back({y, "Solar Array", "Partial dust-induced degradation", false});
        }
        if (std::uniform_real_distribution<>(0, 1)(rng_) < base_rate * 0.6) {
            events.push_back({y, "ISRU Plant", "Filter clogging detected", false});
        }
        if (std::uniform_real_distribution<>(0, 1)(rng_) < base_rate * 0.4 && y > 1.0) {
            events.push_back({y, "Medical", "Crew member minor injury during EVA", true});
        }
    }

    return events;
}

} // namespace mars::reliability
