#pragma once

/**
 * @file constants.hpp
 * @brief Foundational physical constants and nominal values for Mars environment.
 *        All values SI units unless noted. Sourced from NASA/peer-reviewed data
 *        (MSL-RAD, Viking, MRO, etc.). See README.md for references and limitations.
 *
 * @note Production notes:
 *  - constexpr for compile-time use where possible.
 *  - Gravity uses user-specified 3.71 m/s² as baseline; actual mean ~3.721 m/s²
 *    with global variation ~±0.03 m/s² (areoid referenced). Local anomalies exist.
 *  - Atmosphere nominals are annual/global averages; strong seasonal (CO2 cycle)
 *    and diurnal variations exist. Pressure range ~400-1200 Pa observed.
 *  - Radiation: GCR baseline from MSL-RAD ~0.64-0.7 mSv/day equiv on surface.
 *    Varies with solar cycle (~50% modulation), pressure/altitude, SPEs.
 *  - Solar: TOA irradiance ~590 W/m² mean; varies ~±20% due to eccentricity.
 *    Surface attenuated by dust (optical depth τ 0.4 clear to >5 storm).
 *
 * Edge cases handled in models: alt < 0 (clamp or warning), P<=0, extreme T.
 * Assumptions flagged inline.
 */

#include <cstdint>
#include <limits>

namespace mars::foundation {

/// Mars mean radius (m) - volumetric mean
inline constexpr double MARS_RADIUS_MEAN = 3'389'500.0;

/// Standard gravitational parameter GM (m³/s²) - for potential models later
inline constexpr double MARS_GM = 4.282837e13;

/// Mean surface gravity (m/s²) - user-specified baseline; see note above
inline constexpr double GRAVITY_MARS = 3.71;

/// More precise mean gravity (m/s²) for reference/comparison
inline constexpr double GRAVITY_MARS_PRECISE = 3.72076;

/// Mars sidereal rotation period (s) ~24.6229 h
inline constexpr double MARS_SOL_LENGTH_S = 88'775.244;

/// Average solar irradiance at Mars distance (W/m²) - TOA, mean distance
inline constexpr double SOLAR_CONSTANT_MARS = 590.0;

/// Mars semi-major axis (AU) for eccentricity calc reference
inline constexpr double MARS_SEMI_MAJOR_AU = 1.52368;

/// Nominal surface pressure (Pa) - global annual avg ~6.1 mbar
inline constexpr double ATM_PRESSURE_SURFACE_PA = 610.0;

/// Nominal surface temperature (K) - global avg ~210 K (-63°C)
inline constexpr double ATM_TEMPERATURE_SURFACE_K = 210.0;

/// Atmospheric scale height approx (m) for simple hydrostatic model
/// H = R_specific * T / g ; R_CO2 ~188.9 J/kgK but effective ~11 km
inline constexpr double ATM_SCALE_HEIGHT_M = 10'800.0;

/// Nominal surface density (kg/m³) - ~0.015-0.02
inline constexpr double ATM_DENSITY_SURFACE_KGM3 = 0.0155;

/// GCR absorbed dose rate surface avg (uGy/day) - varies 200-320+
inline constexpr double GCR_ABSORBED_DOSE_RATE_UGY_D = 250.0;

/// GCR dose equivalent rate surface avg (mSv/day) - MSL-RAD ~0.67
inline constexpr double GCR_DOSE_EQUIV_RATE_MSV_D = 0.67;

/// Quality factor typical for GCR on Mars surface
inline constexpr double GCR_QUALITY_FACTOR = 2.7;

/// Linear pressure shielding coeff approx (uGy/day per Pa) from RAD data
inline constexpr double RAD_PRESSURE_SHIELDING_COEFF = 0.15;

/// Solar energetic particle (SEP) event flag/threshold (example high)
inline constexpr double SEP_DOSE_RATE_MSV_D = 10.0; // illustrative; rare

/// Maximum credible optical depth for dust (storms)
inline constexpr double DUST_TAU_MAX = 8.0;

/// Clear sky optical depth baseline
inline constexpr double DUST_TAU_CLEAR = 0.5;

/// Physical constants (std)
inline constexpr double PI = 3.14159265358979323846;
inline constexpr double DEG_TO_RAD = PI / 180.0;
inline constexpr double RAD_TO_DEG = 180.0 / PI;
inline constexpr double G_EARTH = 9.80665; // for relative calcs

// Limits for robustness
inline constexpr double ALTITUDE_MIN_M = -10'000.0; // Hellas basin ~ -7.2km
inline constexpr double ALTITUDE_MAX_M = 25'000.0;  // Olympus ~21.9km
inline constexpr double PRESSURE_MIN_PA = 10.0;
inline constexpr double PRESSURE_MAX_PA = 1'200.0;

} // namespace mars::foundation
