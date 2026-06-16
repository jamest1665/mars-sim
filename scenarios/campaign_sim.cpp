/**
 * @file campaign_sim.cpp
 * @brief Campaign Simulator with risk, resources, and dashboard.
 */

#include "mars/campaign/campaign.hpp"

#include <iomanip>
#include <iostream>

using namespace mars::campaign;
using namespace mars::agents;

int main() {
    std::cout << "=== Mars Campaign Simulator v1.2 ===\n";
    std::cout << "Risk, Resources, Cost & What-If Ready\n\n";

    CampaignSimulator sim;
    auto result = sim.run_campaign(15.0, AgentStrategy::BALANCED, 6, 2.5);

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Year | Pop | Bone% | Risk | O2(kg) | Water(kg) | Cost($M) | Supply?\n";
    std::cout << "-----|-----|-------|------|--------|-----------|----------|--------\n";

    for (const auto& y : result.yearly_data) {
        std::cout << std::setw(4) << y.year << " | "
                  << std::setw(3) << y.population << " | "
                  << std::setw(5) << y.bone_density_avg << " | "
                  << std::setw(4) << y.cumulative_risk << " | "
                  << std::setw(6) << y.o2_stockpile_kg << " | "
                  << std::setw(9) << y.water_stockpile_kg << " | "
                  << std::setw(8) << y.mission_cost_musd << " | "
                  << (y.supply_ship_arrived ? "Yes" : "-") << "\n";
    }

    std::cout << "\n=== Summary ===\n";
    std::cout << "Final Pop: " << result.yearly_data.back().population << " | "
              << "Risk: " << result.final_risk_score << " | "
              << "Cost: $" << int(result.total_cost_musd) << "M | "
              << (result.campaign_success ? "SUCCESS" : "FAIL") << "\n";

    return 0;
}
