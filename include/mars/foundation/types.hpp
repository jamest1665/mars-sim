#pragma once

/**
 * @file types.hpp
 * @brief Common data types and structures for Mars foundation models.
 *        Lightweight, no virtuals for performance. Extensible via composition.
 */

#include <array>
#include <cmath>
#include <cstdint>
#include <string_view>

#include "constants.hpp"

namespace mars::foundation {

/// 3D vector (SI units, e.g. position m, velocity m/s, accel m/s²)
struct Vec3 {
    double x{0.0};
    double y{0.0};
    double z{0.0};

    [[nodiscard]] constexpr double norm() const noexcept {
        return std::sqrt(x*x + y*y + z*z);
    }
};

/// Areocentric latitude/longitude/altitude (deg, deg, m)
/// Lat: -90 to +90, Lon: -180 to 180 or 0-360
struct LatLonAlt {
    double latitude_deg{0.0};
    double longitude_deg{0.0};
    double altitude_m{0.0};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return latitude_deg >= -90.0 && latitude_deg <= 90.0 &&
               altitude_m >= ALTITUDE_MIN_M && altitude_m <= ALTITUDE_MAX_M;
    }
};

/// Basic atmospheric state at a point
struct AtmosphereState {
    double pressure_pa{ATM_PRESSURE_SURFACE_PA};
    double temperature_k{ATM_TEMPERATURE_SURFACE_K};
    double density_kgm3{ATM_DENSITY_SURFACE_KGM3};
    double scale_height_m{ATM_SCALE_HEIGHT_M};
    // Future: wind components, humidity (trace), dust loading
};

/// Radiation environment snapshot (focus GCR + simple SEP flag)
struct RadiationState {
    double gcr_absorbed_ugy_per_day{GCR_ABSORBED_DOSE_RATE_UGY_D};
    double gcr_dose_equiv_msv_per_day{GCR_DOSE_EQUIV_RATE_MSV_D};
    double quality_factor{GCR_QUALITY_FACTOR};
    bool sep_event_active{false};
    double total_dose_equiv_msv_per_day{GCR_DOSE_EQUIV_RATE_MSV_D};
    // Future: LET spectrum, secondary particles, regolith albedo contrib
};

/// Solar insolation at surface or TOA
struct SolarState {
    double toa_irradiance_wm2{SOLAR_CONSTANT_MARS};      // top of atmosphere
    double direct_beam_wm2{0.0};                         // attenuated direct
    double diffuse_wm2{0.0};                             // scattered
    double global_horizontal_wm2{0.0};                   // total on flat surface
    double optical_depth_tau{DUST_TAU_CLEAR};
    double zenith_angle_deg{0.0};
};

/// Full environmental snapshot at location/time (extensible)
struct EnvironmentState {
    LatLonAlt location{};
    AtmosphereState atmosphere{};
    RadiationState radiation{};
    SolarState solar{};
    double gravity_mps2{GRAVITY_MARS};
    double local_time_sol{0.5}; // 0-1 fraction of sol
    double solar_longitude_ls_deg{0.0}; // season 0-360
    // Future: magnetic field (none), regolith props, etc.
};

} // namespace mars::foundation
