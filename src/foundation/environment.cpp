/**
 * @file environment.cpp
 * @brief Implementation of MarsEnvironment foundational models.
 *        Accuracy prioritized; all models documented with assumptions/edge cases.
 *        Performance: minimal branches, no allocations in hot paths, double math.
 */

#include "mars/foundation/environment.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace mars::foundation {

namespace {
// Local constants for impl (R for CO2 dominant ~188.9 J/kgK, but tuned to match nominal density)
constexpr double R_SPECIFIC_CO2 = 188.9; // J/(kg·K)
constexpr double MARS_ORBITAL_ECC = 0.0934;
} // anon

MarsEnvironment::MarsEnvironment() : MarsEnvironment(Config{}) {}

MarsEnvironment::MarsEnvironment(Config cfg) : m_cfg(std::move(cfg)) {
    // Clamp/validate config on construction (production robustness)
    m_cfg.gravity_mps2 = std::max(3.5, std::min(3.9, m_cfg.gravity_mps2));
    m_cfg.base_pressure_pa = std::max(PRESSURE_MIN_PA, std::min(PRESSURE_MAX_PA, m_cfg.base_pressure_pa));
    m_cfg.scale_height_m = std::max(8'000.0, std::min(15'000.0, m_cfg.scale_height_m));
}

double MarsEnvironment::pressure_at_altitude(double altitude_m) const noexcept {
    // Hydrostatic: P = P0 * exp(-alt / H)
    // Assumption: constant T, g, composition. Good first-order for <20km.
    // Edge: alt out of range -> log warning? but noexcept so clamp + extrapolate
    const double alt = std::clamp(altitude_m, ALTITUDE_MIN_M, ALTITUDE_MAX_M);
    const double exponent = -alt / m_cfg.scale_height_m;
    // Prevent under/overflow
    if (exponent > 20.0) return PRESSURE_MIN_PA;
    if (exponent < -20.0) return m_cfg.base_pressure_pa * 2.0; // rare
    return m_cfg.base_pressure_pa * std::exp(exponent);
}

double MarsEnvironment::temperature_at_altitude(double altitude_m) const noexcept {
    // v1: simple constant or weak lapse. Real Mars has complex profile (inversion etc.)
    // Assumption: isothermal for foundation simplicity. Lapse ~ -1 to -2K/km in lower atm.
    // Future module can override with full profile.
    const double alt = std::clamp(altitude_m, ALTITUDE_MIN_M, ALTITUDE_MAX_M);
    constexpr double lapse_rate_k_per_m = -0.0015; // weak average
    return m_cfg.base_temp_k + alt * lapse_rate_k_per_m;
}

double MarsEnvironment::density_at(double altitude_m) const noexcept {
    // Ideal gas: rho = P / (R T)
    const double p = pressure_at_altitude(altitude_m);
    const double t = temperature_at_altitude(altitude_m);
    if (t <= 0.0) return 0.0;
    return p / (R_SPECIFIC_CO2 * t);
}

AtmosphereState MarsEnvironment::atmosphere_at(double altitude_m) const noexcept {
    AtmosphereState s;
    s.pressure_pa = pressure_at_altitude(altitude_m);
    s.temperature_k = temperature_at_altitude(altitude_m);
    s.density_kgm3 = density_at(altitude_m);
    s.scale_height_m = m_cfg.scale_height_m;
    return s;
}

RadiationState MarsEnvironment::radiation_at(const LatLonAlt& loc,
                                             double pressure_pa,
                                             bool sep_active) const noexcept {
    RadiationState rad;
    const double p = (pressure_pa > 0.0) ? pressure_pa : pressure_at_altitude(loc.altitude_m);

    // GCR baseline adjusted by pressure shielding (higher P -> lower dose, empirical from RAD ~0.15 uGy/d/Pa)
    // pressure_factor keeps it bounded; real physics includes column depth, solar cycle, albedo.
    const double pressure_factor = std::max(0.4, 1.0 - 0.00015 * (p - m_cfg.base_pressure_pa));
    rad.gcr_absorbed_ugy_per_day = std::max(50.0, GCR_ABSORBED_DOSE_RATE_UGY_D * pressure_factor);
    rad.gcr_dose_equiv_msv_per_day = m_cfg.base_gcr_dose_msv_d * pressure_factor;
    rad.quality_factor = GCR_QUALITY_FACTOR;
    rad.sep_event_active = sep_active;
    rad.total_dose_equiv_msv_per_day = sep_active ?
        rad.gcr_dose_equiv_msv_per_day + SEP_DOSE_RATE_MSV_D :
        rad.gcr_dose_equiv_msv_per_day;

    // Note: ignores solar cycle modulation, altitude regolith, SPE spectrum hardening.
    // Real MSL data shows ~±50% variation over cycle + seasonal P effect ~10-20%.
    return rad;
}

double MarsEnvironment::compute_zenith_angle(const LatLonAlt& loc, double ls_deg, double time_frac) const noexcept {
    // Simplified: declination from ls, hour angle from time_frac.
    // Real: uses sub-solar point, equation of time. Good enough for insolation estimate.
    // ls=0 northern spring equinox. Decl ~ 25.19 * sin(ls) deg obliquity.
    constexpr double obliquity = 25.19 * DEG_TO_RAD;
    const double decl = obliquity * std::sin(ls_deg * DEG_TO_RAD);
    const double lat_rad = loc.latitude_deg * DEG_TO_RAD;
    const double ha = (time_frac - 0.5) * 2.0 * PI; // hour angle, noon=0
    const double cos_zen = std::sin(lat_rad) * std::sin(decl) +
                           std::cos(lat_rad) * std::cos(decl) * std::cos(ha);
    return std::acos(std::clamp(cos_zen, -1.0, 1.0)) * RAD_TO_DEG;
}

double MarsEnvironment::compute_direct_attenuation(double tau, double zenith_rad) const noexcept {
    // Beer-Lambert: direct = exp(-tau / mu) where mu=cos(zenith) approx for flat.
    // Edge: zenith >89 deg -> near 0; tau high -> 0.
    if (zenith_rad > (89.0 * DEG_TO_RAD)) return 0.0;
    const double mu = std::max(0.01, std::cos(zenith_rad));
    return std::exp(-tau / mu);
}

SolarState MarsEnvironment::solar_at(const LatLonAlt& loc,
                                     double ls_deg,
                                     double local_time_frac,
                                     std::optional<double> tau_override) const noexcept {
    SolarState sol;
    sol.toa_irradiance_wm2 = SOLAR_CONSTANT_MARS *
        (1.0 + MARS_ORBITAL_ECC * std::cos(ls_deg * DEG_TO_RAD)); // rough ecc variation ~±9%

    const double tau = tau_override.value_or(m_cfg.base_tau);
    sol.optical_depth_tau = tau;
    sol.zenith_angle_deg = compute_zenith_angle(loc, ls_deg, local_time_frac);

    const double zenith_rad = sol.zenith_angle_deg * DEG_TO_RAD;
    const double direct = sol.toa_irradiance_wm2 * compute_direct_attenuation(tau, zenith_rad);
    sol.direct_beam_wm2 = direct;

    // Rough diffuse: ~10-30% of attenuated TOA for Mars dust (empirical)
    const double attenuated = sol.toa_irradiance_wm2 * (1.0 - std::exp(-tau));
    sol.diffuse_wm2 = attenuated * 0.25; // tunable placeholder

    sol.global_horizontal_wm2 = sol.direct_beam_wm2 + sol.diffuse_wm2;

    // Night side
    if (sol.zenith_angle_deg > 90.0) {
        sol.direct_beam_wm2 = 0.0;
        sol.diffuse_wm2 *= 0.1; // residual?
        sol.global_horizontal_wm2 = sol.diffuse_wm2;
    }

    return sol;
}

EnvironmentState MarsEnvironment::sample_state(const LatLonAlt& loc,
                                               double ls_deg,
                                               double local_time_frac,
                                               bool sep_active) const noexcept {
    EnvironmentState state;
    state.location = loc;
    if (!validate_location(loc)) {
        // In production: could log or return partial; here clamp silently for robustness
        state.location.altitude_m = std::clamp(loc.altitude_m, ALTITUDE_MIN_M, ALTITUDE_MAX_M);
    }
    state.atmosphere = atmosphere_at(state.location.altitude_m);
    state.radiation = radiation_at(state.location, state.atmosphere.pressure_pa, sep_active);
    state.solar = solar_at(state.location, ls_deg, local_time_frac);
    state.gravity_mps2 = m_cfg.gravity_mps2;
    state.local_time_sol = local_time_frac;
    state.solar_longitude_ls_deg = ls_deg;
    return state;
}

bool MarsEnvironment::validate_location(const LatLonAlt& loc) const noexcept {
    return loc.is_valid() &&
           loc.longitude_deg >= -180.0 && loc.longitude_deg <= 360.0;
}

} // namespace mars::foundation
