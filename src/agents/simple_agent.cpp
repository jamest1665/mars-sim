/**
 * @file simple_agent.cpp
 * @brief Example rule-based agent for habitat management.
 */

#include "mars/agents/agent.hpp"

#include <iostream>

namespace mars::agents {

class SimpleHabitatAgent : public Agent {
public:
    void act(const mars::foundation::EnvironmentState& state,
             const mars::habitats::ResourceState& resources,
             double /*years_elapsed*/) override {
        // Simple rule: if power is low and solar is decent, prioritize solar arrays
        if (resources.power_generation_kw < resources.power_demand_kw * 0.8 &&
            state.solar.global_horizontal_wm2 > 100) {
            // In a real system this would adjust habitat power allocation
        }

        // If O2 is low, increase ISRU effort (simulated)
        if (resources.o2_production_kg_per_day < 5.0) {
            // Would trigger higher ISRU mode
        }
    }

    [[nodiscard]] std::string_view name() const override {
        return "SimpleHabitatAgent";
    }
};

} // namespace mars::agents
