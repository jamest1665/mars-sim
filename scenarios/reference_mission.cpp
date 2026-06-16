/**
 * @file reference_mission.cpp
 * @brief Example reference mission: 3-year crewed surface outpost.
 */

#include "mars/mission/orchestrator.hpp"
#include "mars/reliability/reliability.hpp"

#include <iostream>

int main() {
    std::cout << "=== Reference Mission: 3-Year Crewed Outpost ===\n\n";

    mars::mission::MissionOrchestrator mission;
    auto events = mission.run_mission(3.0, true);

    std::cout << "Mission Events (" << events.size() << " total):\n";
    for (const auto& e : events) {
        std::cout << "  Year " << e.year << ": " << e.description;
        if (e.is_critical) std::cout << " [CRITICAL]";
        std::cout << "\n";
    }

    // Reliability overlay
    mars::reliability::ReliabilityModel reliability;
    auto failures = reliability.simulate_failures(3.0);

    std::cout << "\nSimulated Failures (" << failures.size() << "):\n";
    for (const auto& f : failures) {
        std::cout << "  Year " << f.year << ": " << f.component << " - " << f.description << "\n";
    }

    std::cout << "\nReference mission complete.\n";
    return 0;
}
