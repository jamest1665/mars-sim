/**
 * @file campaign_sim.cpp
 * @brief Full Campaign Simulator demo.
 */

#include "mars/campaign/campaign.hpp"

#include <iomanip>
#include <iostream>

using namespace mars::campaign;
using namespace mars::agents;

int main() {
    std::cout << "=== Mars Campaign Simulator v1.1 ===\n";
    std::cout << "15-year colony campaign with dynamic agent decisions\n\n";

    CampaignSimulator sim;
    auto result = sim.run_campaign(15.0, AgentStrategy::BALANCED, 6, 2.5);

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Year | Pop | Pressure | Bone% | Cancer% | Power Margin | Supply? | Failure?\n";
    std::cout << "-----|-----|----------|-------|---------|--------------|---------|---------\n";

    for (const auto& y : result.yearly_data) {
        std::cout << std::setw(4) << y.year << " | "
                  << std::setw(3) << y.population << " | "
                  << std::setw(8) << y.pressure_kpa << " | "
                  << std::setw(5) << y.bone_density_avg << " | "
                  << std::setw(7) << y.cancer_risk << " | "
                  << std::setw(12) << y.power_margin_kw << " | "
                  << (y.supply_ship_arrived ? "  Yes  " : "   -   ") << " | "
                  << (y.major_failure ? " Yes " : "  -  ") << "\n";
    }

    std::cout << "\n=== Campaign Summary ===\n";
    std::cout << "Final Population:           " << result.yearly_data.back().population << "\n";
    std::cout << "Campaign Duration:          " << result.colony_sustainability_years << " years\n";
    std::cout << "Final Risk Score:           " << result.final_risk_score << "\n";
    std::cout << "Campaign Success:           " << (result.campaign_success ? "YES" : "NO") << "\n";

    std::cout << "\nThis is now a real mission-planning engine.\n";

    return 0;
}
