#pragma once

/**
 * @file constants.hpp
 * @brief Constants for Mars terraforming module.
 *        Order-of-magnitude values drawn from NASA DRA studies, Zubrin concepts,
 *        and peer-reviewed terraforming literature. See README for references.
 *
 * Production notes:
 * - Focus on parametric atmosphere thickening + greenhouse effect.
 * - Radiation shielding improves with increased column mass.
 * - All values are starting points for scenario exploration; real engineering
 *   would require detailed climate modeling.
 */

namespace mars::terraforming {

/// Current Mars surface pressure (Pa) - reference
inline constexpr double CURRENT_PRESSURE_PA = 610.0;

/// Target "habitable" pressure range (Pa) - ~30-50 kPa for shirt-sleeve + liquid water
inline constexpr double TARGET_HABITABLE_PRESSURE_PA = 30'000.0;

/// CO2 molecular mass (kg/mol)
inline constexpr double CO2_MOLAR_MASS = 0.04401;

/// Simple greenhouse sensitivity (K per doubling of pressure, rough)
inline constexpr double GREENHOUSE_SENSITIVITY_K = 5.0;

/// Radiation shielding improvement per 10x pressure increase (fractional GCR reduction)
inline constexpr double RADIATION_SHIELDING_PER_DECADE = 0.25;

/// ISRU energy cost proxy for releasing 1 kg CO2 (kWh/kg, rough)
inline constexpr double ISRU_ENERGY_KWH_PER_KG_CO2 = 2.5;

/// Time scale for noticeable terraforming (Earth years for order-of-magnitude pressure change)
inline constexpr double TERRAFORMING_TIME_SCALE_YEARS = 50.0;

/// Maximum credible pressure (Pa) before runaway or engineering limits
inline constexpr double MAX_CREDIBLE_PRESSURE_PA = 100'000.0;

} // namespace mars::terraforming
