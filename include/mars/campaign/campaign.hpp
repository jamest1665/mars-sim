#pragma once

/**
 * @file campaign.hpp
 * @brief Campaign Simulator - long-term mission planning engine with risk, resources, and what-if.
 */

#include <mars/foundation/environment.hpp>
#include <mars/weather/weather.hpp>
#include <mars/terraforming/terraforming.hpp>
#include <mars/habitats/habitats.hpp>
#include <mars/humans/humans.hpp>
#include <mars/power/power.hpp>
#include <mars/agents/strategies.hpp>
#include <mars/operations/operations.hpp>
#include <mars/reliability/reliability.hpp>

#include <vector>
#include <string>

namespace mars::campaign {

struct CampaignYear {
    double year{0.0};
    int population{6};
    double pressure_kpa{0.0};
    double bone_density_avg{100.0};
    double cancer_risk{0.0};
    double power_margin_kw{0.0};
    double o2_stockpile_kg{0.0};
    double water_stockpile_kg{0.0};
    double propellant_stockpile_kg{0.0};
    double cumulative_risk{0.0};
    bool supply_ship_arrived{false};
    bool major_failure{false};
    double mission_cost_musd{0.0};
    std::string notes;
};

struct CampaignResult {
    std::vector<CampaignYear> yearly_data;
    double final_risk_score{0.0};
    double colony_sustainability_years{0.0};
    bool campaign_success{false};
    double total_cost_musd{0.0};
};

class CampaignSimulator {
public:
    CampaignSimulator() = default;

    CampaignResult run_campaign(double duration_years = 15.0,
                                agents::AgentStrategy strategy = agents::AgentStrategy::BALANCED,
                                int initial_population = 6,
                                double supply_interval_years = 2.0);

    std::vector<CampaignResult> compare_strategies(double duration_years = 12.0,
                                                   int initial_population = 6,
                                                   double supply_interval = 2.5);

private:
    foundation::MarsEnvironment env_;
    weather::MarsWeather weather_;
    terraforming::MarsTerraforming terra_;
    habitats::MarsHabitats habitats_;
    humans::MarsHumans humans_;
    power::MarsPower power_;
    agents::StrategyAgent agent_;
    operations::SurfaceOperations ops_;
    reliability::ReliabilityModel reliability_;
};

} // namespace mars::campaign
