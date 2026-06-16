/**
 * @file humans_demo.cpp
 * @brief Demo of Human Survival Models on the full previous stack.
 *        Shows health trajectory over a 3-year mission with good terraforming/habitats.
 */

#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"
#include "mars/terraforming/terraforming.hpp"
#include "mars/habitats/habitats.hpp"
#include "mars/humans/humans.hpp"

#include <iomanip>
#include <iostream>

using namespace mars::foundation;
using namespace mars::weather;
using namespace mars::terraforming;
using namespace mars::habitats;
using namespace mars::humans;

int main() {
    std::cout << "=== Mars Simulator v0.5 — Human Survival Models Demo ===\n";
    std::cout << "Full stack through Humans (3-year mission)\n\n";

    MarsEnvironment env;
    MarsWeather weather;
    MarsTerraforming terra;
    MarsHabitats habitats;
    MarsHumans humans;

    LatLonAlt site{-30.0, 0.0, 0.0};

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Year | Bone% | Muscle% | Cardio% | Cumul. Dose(Sv) | Cancer Risk% | Nutrition | Psych | Sustainable?\n";
    std::cout << "-----|-------|---------|---------|-----------------|--------------|-----------|-------|-------------\n";

    for (double year : {0.5, 1.0, 2.0, 3.0}) {
        auto state = env.sample_state(site, 270.0, 0.5);
        weather.apply_to_state(state, 270.0, site);
        terra.apply_to_state(state, year, site);
        auto resources = habitats.calculate_resources(state, year);
        auto health = humans.update_health(state, resources, year);

        std::cout << std::setw(4) << year << " | "
                  << std::setw(5) << health.bone_density_percent << " | "
                  << std::setw(7) << health.muscle_mass_percent << " | "
                  << std::setw(7) << health.cardiovascular_fitness << " | "
                  << std::setw(15) << health.cumulative_radiation_dose_sv << " | "
                  << std::setw(12) << health.cancer_risk_increase_percent << " | "
                  << std::setw(9) << health.nutrition_status << " | "
                  << std::setw(5) << health.psychological_wellbeing << " | "
                  << (resources.closed_loop_sustainable ? "   YES   " : "    no   ") << "\n";
    }

    std::cout << "\nWith good terraforming and habitats, human health remains manageable over 3 years.\n";
    std::cout << "Project complete through v0.5. Ready for viz/scenarios or further refinement.\n";

    return 0;
}
