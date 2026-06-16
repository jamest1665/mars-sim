#pragma once

/**
 * @file constants.hpp
 * @brief Constants for Mars habitats + resource loops module.
 *        Based on NASA DRA 5.0, Mars One concepts, and ECLSS literature.
 */

namespace mars::habitats {

/// Crew size default
inline constexpr int DEFAULT_CREW_SIZE = 6;

/// Daily O2 consumption per person (kg/day)
inline constexpr double O2_CONSUMPTION_KG_PER_PERSON_DAY = 0.84;

/// Daily water consumption per person (kg/day, including hygiene)
inline constexpr double WATER_CONSUMPTION_KG_PER_PERSON_DAY = 3.5;

/// ECLSS water recycling efficiency (modern target)
inline constexpr double ECLSS_WATER_RECYCLE_EFFICIENCY = 0.95;

/// Sabatier + electrolysis O2 yield from CO2 (kg O2 per kg CO2)
inline constexpr double SABATIER_O2_YIELD = 0.727;

/// Regolith water extraction yield (kg water per kg regolith, optimistic)
inline constexpr double REGOLITH_WATER_YIELD = 0.05;

/// Solar power degradation due to dust/atm (fraction of Earth orbit)
inline constexpr double MARS_SOLAR_POWER_FACTOR = 0.43;

/// Nuclear power plant specific mass (kg/kW, rough for surface reactor)
inline constexpr double NUCLEAR_POWER_SPECIFIC_MASS_KG_PER_KW = 50.0;

/// Habitat shielding mass per m² per Sv/year reduction needed (kg)
inline constexpr double SHIELDING_MASS_KG_PER_M2_PER_SV = 150.0;

/// Baseline habitat mass per crew member (kg, structure + systems, no shielding)
inline constexpr double BASE_HABITAT_MASS_PER_PERSON_KG = 8000.0;

} // namespace mars::habitats
