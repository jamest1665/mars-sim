#pragma once

/**
 * @file weather.hpp
 * @brief MarsWeather module: parametric dust optical depth, storm events,
 *        and lifting potential. Designed to compose cleanly with
 *        mars::foundation::MarsEnvironment and EnvironmentState.
 *
 * Key capabilities:
 * - Seasonal (Ls) and latitudinal dust loading
 * - Optional stochastic or threshold-based global/regional storms
 * - Direct application to foundation EnvironmentState (updates τ and solar/rad)
 * - O(1) queries, noexcept, production hardened
 *
 * Integration example:
 *   mars::foundation::MarsEnvironment env;
 *   mars::weather::MarsWeather weather;
 *   auto state = env.sample_state(loc, ls);
 *   weather.apply_to_state(state, ls, loc);
 *   // now state.solar uses updated dust loading
 */

#include "constants.hpp"
#include "types.hpp"

#include <mars/foundation/types.hpp>
#include <optional>
#include <random>
#include <string_view>

namespace mars::weather {

class MarsWeather {
public:
    explicit MarsWeather(WeatherConfig cfg = {});
    ~MarsWeather() = default;

    MarsWeather(const MarsWeather&) = default;
    MarsWeather& operator=(const MarsWeather&) = default;

    /// Core query: optical depth at location and season
    /// Combines seasonal curve + latitudinal weighting + optional storm spike
    [[nodiscard]] double optical_depth(double ls_deg,
                                       double latitude_deg,
                                       double longitude_deg = 0.0,
                                       double local_time_frac = 0.5) const noexcept;

    /// Dust + storm state snapshot
    [[nodiscard]] DustState sample_dust(const mars::foundation::LatLonAlt& loc,
                                        double ls_deg,
                                        double local_time_frac = 0.5) const noexcept;

    /// Apply dust effects directly to a foundation EnvironmentState
    /// Updates solar optical depth and can flag storm conditions.
    /// Non-mutating on foundation internals.
    void apply_to_state(mars::foundation::EnvironmentState& state,
                        double ls_deg,
                        const mars::foundation::LatLonAlt& loc) const noexcept;

    /// Simple storm activity flag (threshold or probabilistic)
    [[nodiscard]] bool is_storm_active(double ls_deg, double latitude_deg) const noexcept;

    /// Lifting potential proxy (0-1) — useful for future ISRU / habitat dust mitigation
    [[nodiscard]] double lifting_potential(double ls_deg, double latitude_deg,
                                           double local_time_frac = 0.5) const noexcept;

    void set_config(const WeatherConfig& cfg) noexcept { m_cfg = cfg; }
    [[nodiscard]] const WeatherConfig& config() const noexcept { return m_cfg; }

    [[nodiscard]] std::string_view model_description() const noexcept {
        return "Mars weather v0.2: parametric seasonal dust + storm model (MGS/MRO calibrated)";
    }

private:
    WeatherConfig m_cfg;
    mutable std::mt19937 m_rng;  // for stochastic storms if enabled

    [[nodiscard]] double seasonal_tau_factor(double ls_deg) const noexcept;
    [[nodiscard]] double latitudinal_weight(double latitude_deg, double ls_deg) const noexcept;
    [[nodiscard]] double storm_multiplier(double ls_deg, double latitude_deg) const noexcept;
};

} // namespace mars::weather
