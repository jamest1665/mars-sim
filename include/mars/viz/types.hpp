#pragma once

/**
 * @file types.hpp
 * @brief Data types for viz and scenarios.
 */

#include <vector>
#include <string>

namespace mars::viz {

struct TimeSeriesPoint {
    double year{0.0};
    double pressure_kpa{0.0};
    double temperature_k{0.0};
    double gcr_dose_msv_per_day{0.0};
    double solar_global_wm2{0.0};
    double o2_production_kg_per_day{0.0};
    double habitat_mass_tonnes{0.0};
    double bone_density_percent{100.0};
    double cancer_risk_increase{0.0};
    bool habitat_sustainable{false};
};

struct ScenarioResult {
    std::vector<TimeSeriesPoint> time_series;
    double final_bone_density{0.0};
    double final_cancer_risk{0.0};
    bool mission_successful{false};
    std::string notes;
};

} // namespace mars::viz
