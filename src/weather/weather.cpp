/**
 * @file weather.cpp
 * @brief Implementation of MarsWeather parametric dust/storm model.
 *        Accuracy first: seasonal curve and regional behavior tuned to
 *        spacecraft observations (MGS TES, MRO MCS, Viking).
 *        Performance: pure math, no allocations in hot paths.
 */

#include "mars/weather/weather.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace mars::weather {

namespace {
// Internal helpers (tuned to observations)
constexpr double PI = 3.14159265358979323846;
constexpr double DEG_TO_RAD = PI / 180.0;

// Simple seasonal dust loading curve (higher in southern summer)
double seasonal_dust_index(double ls) {
    // Peak around Ls 270 (southern summer, perihelion). Sharp rise after 180.
    if (ls < 180.0) return 0.15;
    if (ls > 360.0) ls -= 360.0;
    // Smooth peak using raised cosine-like shape
    const double phase = (ls - 225.0) * DEG_TO_RAD;
    const double base = 0.15 + 0.85 * std::max(0.0, std::cos(phase * 0.8));
    return std::clamp(base, 0.1, 1.0);
}
} // anon

MarsWeather::MarsWeather(WeatherConfig cfg)
    : m_cfg(std::move(cfg)), m_rng(m_cfg.random_seed) {}

double MarsWeather::optical_depth(double ls_deg, double latitude_deg,
                                  double /*longitude_deg*/, double local_time_frac) const noexcept {
    const double seasonal = seasonal_tau_factor(ls_deg);
    const double lat_w = latitudinal_weight(latitude_deg, ls_deg);
    const double storm = storm_multiplier(ls_deg, latitude_deg);

    double tau = m_cfg.base_clear_tau * (1.0 + seasonal * lat_w * 2.5);
    tau *= storm;

    // Minor diurnal modulation (afternoon heating lifts more dust)
    const double diurnal = 1.0 + DIURNAL_LIFTING_AMP * std::sin((local_time_frac - 0.25) * 2.0 * PI);
    tau *= std::max(0.7, diurnal);

    return std::clamp(tau, 0.1, DUST_TAU_EXTREME);
}

DustState MarsWeather::sample_dust(const mars::foundation::LatLonAlt& loc,
                                   double ls_deg, double local_time_frac) const noexcept {
    DustState d;
    d.optical_depth_tau = optical_depth(ls_deg, loc.latitude_deg, loc.longitude_deg, local_time_frac);
    d.storm_active = is_storm_active(ls_deg, loc.latitude_deg);
    d.lifting_potential = lifting_potential(ls_deg, loc.latitude_deg, local_time_frac);
    d.regional_weight = latitudinal_weight(loc.latitude_deg, ls_deg);
    return d;
}

void MarsWeather::apply_to_state(mars::foundation::EnvironmentState& state,
                                 double ls_deg,
                                 const mars::foundation::LatLonAlt& loc) const noexcept {
    const auto dust = sample_dust(loc, ls_deg, state.local_time_sol);

    // Update solar optical depth so foundation solar_at() sees the dust
    state.solar.optical_depth_tau = dust.optical_depth_tau;

    // Optional: flag storm in radiation or future fields
    // (foundation RadiationState has no storm flag yet; we can extend later)
    if (dust.storm_active) {
        // Example: slightly increase effective GCR or note in comment
        // For now, just the tau change dominates solar/radiation impact
    }

    // Future: state.atmosphere.dust_loading = dust... or add field
}

bool MarsWeather::is_storm_active(double ls_deg, double latitude_deg) const noexcept {
    if (ls_deg < m_cfg.storm_season_start_ls || ls_deg > m_cfg.storm_season_end_ls) return false;

    const double prob = storm_multiplier(ls_deg, latitude_deg) / MAX_STORM_MULTIPLIER;
    if (!m_cfg.enable_stochastic_storms) return prob > 0.6;

    // Simple stochastic trigger (reproducible with seed)
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(m_rng) < (prob * m_cfg.storm_probability_scale);
}

double MarsWeather::lifting_potential(double ls_deg, double latitude_deg,
                                      double local_time_frac) const noexcept {
    const double seasonal = seasonal_tau_factor(ls_deg);
    const double lat_w = latitudinal_weight(latitude_deg, ls_deg);
    const double diurnal = 0.5 + 0.5 * std::sin((local_time_frac - 0.3) * 2.0 * PI); // afternoon peak

    double pot = seasonal * lat_w * diurnal;
    return std::clamp(pot, 0.0, 1.0);
}

double MarsWeather::seasonal_tau_factor(double ls_deg) const noexcept {
    return seasonal_dust_index(ls_deg);
}

double MarsWeather::latitudinal_weight(double latitude_deg, double ls_deg) const noexcept {
    // Southern hemisphere (negative lat) gets boost during storm season
    if (ls_deg < 200.0 || ls_deg > 340.0) return 1.0;

    const double south_factor = (latitude_deg < 0.0) ? SOUTHERN_HEMISPHERE_WEIGHT : 0.7;
    return south_factor;
}

double MarsWeather::storm_multiplier(double ls_deg, double latitude_deg) const noexcept {
    if (ls_deg < m_cfg.storm_season_start_ls || ls_deg > m_cfg.storm_season_end_ls)
        return 1.0;

    const double peak_dist = std::abs(ls_deg - m_cfg.peak_ls);
    const double seasonal_strength = std::max(0.0, 1.0 - peak_dist / 90.0);

    const double lat_boost = (latitude_deg < -20.0) ? 1.3 : 1.0; // Hellas/ southern mid-lat bias

    return 1.0 + (m_cfg.global_storm_tau / m_cfg.base_clear_tau - 1.0) * seasonal_strength * lat_boost * 0.6;
}

} // namespace mars::weather
