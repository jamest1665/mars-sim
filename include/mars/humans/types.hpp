#pragma once

/**
 * @file types.hpp
 * @brief Data types for Mars human survival models.
 */

#include "constants.hpp"

namespace mars::humans {

struct HumanHealthState {
    double bone_density_percent{100.0};
    double muscle_mass_percent{100.0};
    double cardiovascular_fitness{100.0};
    double cumulative_radiation_dose_sv{0.0};
    double cancer_risk_increase_percent{0.0};
    double nutrition_status{100.0}; // 0-100
    double psychological_wellbeing{80.0}; // 0-100
    bool acute_radiation_syndrome_risk{false};
};

struct HumanConfig {
    double mission_duration_years = 3.0;
    double exercise_compliance = 0.8; // countermeasure effectiveness
    double nutrition_efficiency = 0.9;
    bool advanced_psych_support = true;
};

} // namespace mars::humans
