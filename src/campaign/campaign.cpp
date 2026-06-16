/**
 * @file campaign.cpp
 * @brief Implementation of the Campaign Simulator.
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
            if (supply_arrived) {
                population += 4;
            }
        }

        auto failures = reliability_.simulate_failures(0.25);
        bool major_failure = !failures.empty() && failures.back().critical;

        CampaignYear cy;
        cy.year = year;
        cy.population = population;
        cy.pressure_kpa = state.atmosphere.pressure_pa / 1000.0;
        cy.bone_density_avg = health.bone_density_percent;
        cy.cancer_risk = health.cancer_risk_increase_percent;
        cy.power_margin_kw = power_.state().total_available_kw - demand;
        cy.supply_ship_arrived = supply_arrived;
        cy.major_failure = major_failure;
        cy.notes = supply_arrived ? "Supply ship arrived" : (major_failure ? "Critical failure" : "");

        result.yearly_data.push_back(cy);

        if (population > 50 || cy.bone_density_avg < 65.0 || cy.power_margin_kw < -10.0) {
            break;
        }

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

} // namespace mars::campaign
