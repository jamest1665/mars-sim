#pragma once

/**
 * @file types.hpp
 * @brief Data types for Mars weather/dust module.
 *        Lightweight PODs for performance and easy composition with foundation.
 */

#include "constants.hpp"
#include <mars/foundation/types.hpp>  // reuse LatLonAlt, EnvironmentState if needed

namespace mars::weather {

/// Snapshot of dust loading and storm state at a location/time
struct DustState {
    double optical_depth_tau{DUST_TAU_CLEAR};
    bool storm_active{false};
    double lifting_potential{0.0};   // 0-1 proxy for surface wind stress / convection
    double regional_weight{1.0};     // lat/lon source region modifier
};

/// Configuration for weather model fidelity and scenario
struct WeatherConfig {
    double base_clear_tau = DUST_TAU_CLEAR;
    double global_storm_tau = DUST_TAU_GLOBAL_STORM;
    double storm_season_start_ls = STORM_SEASON_START_LS;
    double storm_season_end_ls = STORM_SEASON_END_LS;
    double peak_ls = STORM_PEAK_LS;
    bool enable_stochastic_storms = true;
    double storm_probability_scale = 1.0;  // multiplier for event likelihood
    unsigned int random_seed = 42;         // for reproducible ensembles
    // Future: full wind field, multiple storm centers, GCM coupling hooks
};

} // namespace mars::weather
