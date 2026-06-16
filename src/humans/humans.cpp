/**
 * @file humans.cpp
 * @brief Production implementation of MarsHumans health models.
 */

#include "mars/humans/humans.hpp"

#include <algorithm>
#include <cmath>

namespace mars::humans {

MarsHumans::MarsHumans(HumanConfig cfg) : m_cfg(std::move(cfg)) {}

HumanHealthState MarsHumans::update_health(
    const mars::foundation::EnvironmentState& env_state,
    const mars::habitats::ResourceState& resources,
    double years_elapsed) const noexcept {

    HumanHealthState health;

    // Radiation cumulative + risk (with simple pharma countermeasure modeling)
    const double annual_dose_sv = env_state.radiation.gcr_dose_equiv_msv_per_day * 365.25 / 1000.0;
    const double effective_dose = annual_dose_sv * (1.0 - PHARMA_COUNTERMEASURE_EFFICIENCY * 0.6);
    health.cumulative_radiation_dose_sv = effective_dose * years_elapsed;
    health.cancer_risk_increase_percent = health.cumulative_radiation_dose_sv * CANCER_RISK_PER_SV;
    health.acute_radiation_syndrome_risk = health.cumulative_radiation_dose_sv > ARS_THRESHOLD_SV;

    // Low-g physiology (bone, muscle, cardio) - mitigated by exercise
    const double phys_impact = low_g_physiology_impact(years_elapsed, m_cfg.exercise_compliance);
    health.bone_density_percent = 100.0 - phys_impact * BONE_LOSS_RATE_MARS_G * 12.0 * years_elapsed;
    health.muscle_mass_percent = 100.0 - phys_impact * MUSCLE_ATROPHY_RATE * 12.0 * years_elapsed;
    health.cardiovascular_fitness = 100.0 - phys_impact * CARDIO_DECONDITIONING_FACTOR * 100.0 * years_elapsed;

    // Nutrition from habitat resources
    const double nutrition_factor = std::min(1.0, resources.water_production_kg_per_day / (m_cfg.crew_size * 3.0) * m_cfg.nutrition_efficiency);
    health.nutrition_status = 70.0 + nutrition_factor * 30.0;

    // Psychological wellbeing (improved by better habitats/resources + support)
    double psych = BASE_PSYCH_STRESS * 100.0;
    if (resources.closed_loop_sustainable) psych -= 15.0;
    if (m_cfg.advanced_psych_support) psych -= 10.0;
    health.psychological_wellbeing = std::max(40.0, 100.0 - psych);

    // Clamp all
    health.bone_density_percent = std::clamp(health.bone_density_percent, 60.0, 100.0);
    health.muscle_mass_percent = std::clamp(health.muscle_mass_percent, 60.0, 100.0);
    health.cardiovascular_fitness = std::clamp(health.cardiovascular_fitness, 50.0, 100.0);
    health.nutrition_status = std::clamp(health.nutrition_status, 50.0, 100.0);
    health.psychological_wellbeing = std::clamp(health.psychological_wellbeing, 30.0, 100.0);

    return health;
}

double MarsHumans::radiation_health_impact(double dose_sv) const noexcept {
    return dose_sv * CANCER_RISK_PER_SV;
}

double MarsHumans::low_g_physiology_impact(double years, double exercise) const noexcept {
    return std::max(0.2, 1.0 - exercise * 0.6); // better exercise = less impact
}

} // namespace mars::humans
