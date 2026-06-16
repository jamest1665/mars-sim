/**
 * @file viz.cpp
 * @brief Implementation of ScenarioRunner and export utilities.
 *        Ties the entire previous stack into runnable scenarios.
 */

#include "mars/viz/viz.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>
#include <sstream>

namespace mars::viz {

ScenarioResult ScenarioRunner::run_single_scenario(
    double mission_years,
    double terraforming_aggressiveness) const {

    ScenarioResult result;

    mars::foundation::MarsEnvironment env;
    mars::weather::MarsWeather weather;
    mars::terraforming::MarsTerraforming terra;
    mars::habitats::MarsHabitats habitats;
    mars::humans::MarsHumans humans;

    // Tune terraforming aggressiveness
    auto terra_cfg = terra.config();
    terra_cfg.target_pressure_pa *= terraforming_aggressiveness;
    terra.set_config(terra_cfg);

    double current_year = 0.0;
    const double dt = DEFAULT_TIME_STEP_YEARS;

    while (current_year <= mission_years) {
        auto state = env.sample_state({-30.0, 0.0, 0.0}, 270.0, 0.5);
        weather.apply_to_state(state, 270.0, state.location);
        terra.apply_to_state(state, current_year, state.location);

        auto resources = habitats.calculate_resources(state, current_year);
        auto health = humans.update_health(state, resources, current_year);

        TimeSeriesPoint p;
        p.year = current_year;
        p.pressure_kpa = state.atmosphere.pressure_pa / 1000.0;
        p.temperature_k = state.atmosphere.temperature_k;
        p.gcr_dose_msv_per_day = state.radiation.gcr_dose_equiv_msv_per_day;
        p.solar_global_wm2 = state.solar.global_horizontal_wm2;
        p.o2_production_kg_per_day = resources.o2_production_kg_per_day;
        p.habitat_mass_tonnes = resources.habitat_mass_kg / 1000.0;
        p.bone_density_percent = health.bone_density_percent;
        p.cancer_risk_increase = health.cancer_risk_increase_percent;
        p.habitat_sustainable = resources.closed_loop_sustainable;

        result.time_series.push_back(p);

        current_year += dt;
    }

    // Final metrics
    if (!result.time_series.empty()) {
        const auto& last = result.time_series.back();
        result.final_bone_density = last.bone_density_percent;
        result.final_cancer_risk = last.cancer_risk_increase;
        result.mission_successful = (last.bone_density_percent > 75.0) &&
                                    (last.cancer_risk_increase < 15.0) &&
                                    last.habitat_sustainable;
    }

    return result;
}

std::vector<ScenarioResult> ScenarioRunner::run_monte_carlo(
    int num_runs, double mission_years) const {

    std::vector<ScenarioResult> results;
    results.reserve(num_runs);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> terra_dist(0.6, 1.4);

    for (int i = 0; i < num_runs; ++i) {
        double aggressiveness = terra_dist(rng);
        auto res = run_single_scenario(mission_years, aggressiveness);
        res.notes = "terraform_aggressiveness=" + std::to_string(aggressiveness);
        results.push_back(res);
    }

    return results;
}

bool ScenarioRunner::export_to_csv(const ScenarioResult& result, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "year,pressure_kpa,temperature_k,gcr_dose_msv_per_day,solar_wm2,"
         << "o2_kg_per_day,habitat_tonnes,bone_percent,cancer_risk,sustainable\n";

    for (const auto& p : result.time_series) {
        file << p.year << ","
             << p.pressure_kpa << ","
             << p.temperature_k << ","
             << p.gcr_dose_msv_per_day << ","
             << p.solar_global_wm2 << ","
             << p.o2_production_kg_per_day << ","
             << p.habitat_mass_tonnes << ","
             << p.bone_density_percent << ","
             << p.cancer_risk_increase << ","
             << (p.habitat_sustainable ? 1 : 0) << "\n";
    }
    return true;
}

bool ScenarioRunner::export_monte_carlo_summary(const std::vector<ScenarioResult>& results,
                                                const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    int successes = 0;
    double avg_bone = 0.0;
    double avg_cancer = 0.0;

    for (const auto& r : results) {
        if (r.mission_successful) successes++;
        avg_bone += r.final_bone_density;
        avg_cancer += r.final_cancer_risk;
    }

    const size_t n = results.size();
    avg_bone /= n;
    avg_cancer /= n;

    file << "num_runs,success_rate,avg_final_bone_density,avg_cancer_risk_increase\n";
    file << n << ","
         << (static_cast<double>(successes) / n) << ","
         << avg_bone << ","
         << avg_cancer << "\n";

    return true;
}

void ScenarioRunner::print_mission_report(const ScenarioResult& result) {
    if (result.time_series.empty()) {
        std::cout << "No data in scenario result.\n";
        return;
    }

    const auto& start = result.time_series.front();
    const auto& end = result.time_series.back();

    std::cout << "\n=== Mission Report ===\n";
    std::cout << "Duration: " << end.year << " years\n";
    std::cout << "Final Pressure: " << end.pressure_kpa << " kPa\n";
    std::cout << "Final GCR Dose: " << end.gcr_dose_msv_per_day << " mSv/day\n";
    std::cout << "Final Bone Density: " << end.bone_density_percent << "%\n";
    std::cout << "Final Cancer Risk Increase: " << end.cancer_risk_increase << "%\n";
    std::cout << "Habitat Sustainable: " << (end.habitat_sustainable ? "Yes" : "No") << "\n";
    std::cout << "Overall Mission Success: " << (result.mission_successful ? "SUCCESS" : "FAILURE") << "\n";
    std::cout << "========================\n\n";
}

} // namespace mars::viz
