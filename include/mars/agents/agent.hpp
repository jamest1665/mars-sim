#pragma once

/**
 * @file agent.hpp
 * @brief Abstract interface for autonomous agents that can control
 *        habitats, ISRU, and other systems based on environment state.
 */

#include <mars/foundation/types.hpp>
#include <mars/habitats/habitats.hpp>
#include <string_view>

namespace mars::agents {

class Agent {
public:
    virtual ~Agent() = default;

    /// Decide actions based on current environment and resources
    virtual void act(const mars::foundation::EnvironmentState& state,
                     const mars::habitats::ResourceState& resources,
                     double years_elapsed) = 0;

    [[nodiscard]] virtual std::string_view name() const = 0;
};

} // namespace mars::agents
