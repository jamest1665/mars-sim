#pragma once

/**
 * @file constants.hpp
 * @brief Constants for Mars human survival models.
 *        Based on NASA human research, space medicine literature.
 */

namespace mars::humans {

/// Bone mineral density loss rate in Mars gravity (% per month)
inline constexpr double BONE_LOSS_RATE_MARS_G = 0.5;

/// Muscle atrophy rate in low-g (% per month)
inline constexpr double MUSCLE_ATROPHY_RATE = 0.8;

/// Cardiovascular deconditioning factor
inline constexpr double CARDIO_DECONDITIONING_FACTOR = 0.15;

/// Radiation cancer risk (additional % lifetime risk per Sv, rough)
inline constexpr double CANCER_RISK_PER_SV = 5.0;

/// Acute radiation syndrome threshold (Sv)
inline constexpr double ARS_THRESHOLD_SV = 1.0;

/// Daily caloric requirement per person (kcal)
inline constexpr double DAILY_CALORIES_PER_PERSON = 2500.0;

/// Psychological stress factor in confinement (0-1 scale)
inline constexpr double BASE_PSYCH_STRESS = 0.4;

} // namespace mars::humans
