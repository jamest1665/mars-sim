#pragma once

/**
 * @file constants.hpp
 * @brief Constants and nominal values for Mars dust/weather module.
 *        Calibrated to MGS, MRO, Mars Climate Sounder observations and
 *        published dust storm climatology. See README for sources.
 *
 * Production notes:
 * - All in SI where applicable.
 * - Storm season strongly tied to southern spring/summer (Ls 180-360).
 * - Optical depth (τ) is the dominant variable affecting solar attenuation
 *   and surface radiation in the foundation module.
 */

#include <cstdint>

namespace mars::weather {

/// Clear-sky baseline optical depth (visible)
inline constexpr double DUST_TAU_CLEAR = 0.4;

/// Typical global storm peak optical depth
inline constexpr double DUST_TAU_GLOBAL_STORM = 5.0;

/// Extreme global storm (rare, e.g. 2018)
inline constexpr double DUST_TAU_EXTREME = 10.0;

/// Storm season start (Ls degrees) - southern spring
inline constexpr double STORM_SEASON_START_LS = 180.0;

/// Storm season end / peak decline
inline constexpr double STORM_SEASON_END_LS = 360.0;

/// Ls of maximum heating / peak storm probability (southern summer, near perihelion)
inline constexpr double STORM_PEAK_LS = 270.0;

/// Latitude weighting: southern hemisphere amplifies during storm season
inline constexpr double SOUTHERN_HEMISPHERE_WEIGHT = 1.6;

/// Lifting threshold proxy (insolation-based, normalized)
inline constexpr double LIFTING_THRESHOLD = 0.6;

/// Maximum storm multiplier on base tau
inline constexpr double MAX_STORM_MULTIPLIER = 8.0;

/// Simple diurnal modulation amplitude for lifting
inline constexpr double DIURNAL_LIFTING_AMP = 0.3;

/// Quality / confidence note for this parametric model
inline constexpr double MODEL_CONFIDENCE = 0.75; // good first-order; full GCM higher fidelity later

} // namespace mars::weather
