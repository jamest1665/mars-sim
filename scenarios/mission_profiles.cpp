/**
 * @file mission_profiles.cpp
 * @brief Library of reference mission profiles.
 */

#include "mars/mission/orchestrator.hpp"
#include "mars/reliability/reliability.hpp"

#include <iostream>
#include <string>

void run_profile(const std::string& name, double years, bool use_agent) {
    std::cout << "\n=== " << name << " (" << years << " years) ===\n";

    mars::mission::MissionOrchestrator mission;
    auto events = mission.run_mission(years, use_agent);

    int critical = 0;
    for (const auto& e : events) if (e.is_critical) critical++;

    std::cout << "Total events: " << events.size() << " | Critical: " << critical << "\n";

    mars::reliability::ReliabilityModel rel;
    auto failures = rel.simulate_failures(years);
    std::cout << "Simulated failures: " << failures.size() << "\n";
}

int main() {
    std::cout << "=== Mars Simulator v0.9 — Mission Profile Library ===\n";

    run_profile("Short Crewed Outpost", 1.5, true);
    run_profile("Standard 3-Year Crewed Mission", 3.0, true);
    run_profile("Long Duration (5 years) with Conservative Agent", 5.0, true);

    std::cout << "\nAll profiles complete.\n";
    return 0;
}
