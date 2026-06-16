/**
 * @file reliability.cpp
 * @brief Probabilistic reliability, failure modes, and maintenance.
 */

#include "mars/reliability/reliability.hpp"

#include <algorithm>

namespace mars::reliability {

std::vector<FailureEvent> ReliabilityModel::simulate_failures(double years, double /*crew_size*/) {
    std::vector<FailureEvent> events;
    double base_rate = 0.03 * maintenance_factor_;

    for (double y = 0.5; y < years; y += 0.5) {
        if (std::uniform_real_distribution<>(0, 1)(rng_) < base_rate) {
            events.push_back({y, "Solar Array", "Dust-induced degradation", false});
        }
        if (std::uniform_real_distribution<>(0, 1)(rng_) < base_rate * 0.65) {
            events.push_back({y, "ISRU Plant", "Filter clogging / mechanical wear", false});
        }
        if (std::uniform_real_distribution<>(0, 1)(rng_) < base_rate * 0.35 && y > 1.0) {
            events.push_back({y, "Medical", "Crew EVA injury or illness", true});
        }
        if (std::uniform_real_distribution<>(0, 1)(rng_) < base_rate * 0.25) {
            events.push_back({y, "Power Storage", "Battery cell degradation", false});
        }
    }
    return events;
}

void ReliabilityModel::perform_maintenance(double /*year*/, const std::string& /*component*/) {
    maintenance_factor_ = std::max(0.6, maintenance_factor_ * 0.85);
}

} // namespace mars::reliability
