/**
 * @file campaign.cpp
 * @brief Campaign Simulator implementation with risk, resources, and what-if.
 */

#include "mars/campaign/campaign.hpp"

#include <algorithm>
#include <random>

namespace mars::campaign {

CampaignResult CampaignSimulator::run_campaign(double duration_years,
                                               agents::AgentStrategy strategy,
                                               int initial_population,
                                               double supply_interval_years) {

    CampaignResult result;
    std::mt19937 rng(777);

    agent_ = agents::StrategyAgent(strategy);

    double year = 0.0;
    int population = initial_population;
    double next_supply = supply_interval_years;

    // Simple resource stocks
    double o2_stock = 5000.0;
    double water_stock = 12000.0;
    double propellant_stock = 8000.0;

    while (year < duration_years) {
        auto state = env_.sample_state({-30, 0, 0}, 270.0, 0.5);
        weather_.apply_to_state(state, 270.0, state.location);
        terra_.apply_to_state(state, year, state.location);

        auto resources = habitats_.calculate_resources(state, year);
        auto health = humans_.update_health(state, resources, year);

        double demand = resources.power_demand_kw + (population * 2.5);
        power_.update(state, year, demand);

        ops_.update(state, year, population);
        agent_.step(power_.state(), resources, year);

        bool supply_arrived = false;
        if (year >= next_supply) {
            supply_arrived = std::uniform_real_distribution<>(0, 1)(rng) > 0.15;
            next_supply += supply_interval_years;
            if (supply_arrived) population += 4;
        }

        auto failures = reliability_.simulate_failures(0.25);
        bool major_failure = !failures.empty() && failures.back().critical;

        // Resource update
        o2_stock += resources.o2_production_kg_per_day * 180 - (population * 150);
        water_stock += 2000 - (population * 80);
        propellant_stock -= population * 50;

        double current_risk = health.cancer_risk_increase_percent + (major_failure ? 15.0 : 0.0);
        double year_cost = supply_arrived ? 800.0 : 50.0;

        CampaignYear cy;
        cy.year = year;
        cy.population = population;
        cy.pressure_kpa = state.atmosphere.pressure_pa / 1000.0;
        cy.bone_density_avg = health.bone_density_percent;
        cy.cancer_risk = health.cancer_risk_increase_percent;
        cy.power_margin_kw = power_.state().total_available_kw - demand;
        cy.o2_stockpile_kg = std::max(0.0, o2_stock);
        cy.water_stockpile_kg = std::max(0.0, water_stock);
        cy.propellant_stockpile_kg = std::max(0.0, propellant_stock);
        cy.cumulative_risk = current_risk;
        cy.supply_ship_arrived = supply_arrived;
        cy.major_failure = major_failure;
        cy.mission_cost_musd = year_cost;
        cy.notes = supply_arrived ? "Supply arrived" : (major_failure ? "Critical failure" : "");

        result.yearly_data.push_back(cy);
        result.total_cost_musd += year_cost;

        if (population > 50 || cy.bone_density_avg < 65.0 || cy.power_margin_kw < -10.0) break;

        year += 0.5;
    }

    if (!result.yearly_data.empty()) {
        const auto& last = result.yearly_data.back();
        result.final_risk_score = last.cancer_risk + (100.0 - last.bone_density_avg) * 0.5;
        result.colony_sustainability_years = last.year;
        result.campaign_success = (last.population >= initial_population) &&
                                  (last.bone_density_avg > 70.0) &&
                                  (last.power_margin_kw > -5.0);
    }

    return result;
}

std::vector<CampaignResult> CampaignSimulator::compare_strategies(double duration_years,
                                                                  int initial_population,
                                                                  double supply_interval) {
    std::vector<CampaignResult> results;

    std::vector<agents::AgentStrategy> strategies = {
        agents::AgentStrategy::CONSERVATIVE,
        agents::AgentStrategy::BALANCED,
        agents::AgentStrategy::AGGRESSIVE
    };

    for (auto strat : strategies) {
        CampaignSimulator fresh;
        results.push_back(fresh.run_campaign(duration_years, strat, initial_population, supply_interval));
    }

    return results;
}

} // namespace mars::campaign
