#pragma once

/**
 * @file constants.hpp
 * @brief Constants for Mars power systems module.
 */

namespace mars::power {

/// Nominal solar constant at Mars (W/m²)
inline constexpr double MARS_SOLAR_CONSTANT = 590.0;

/// Dust accumulation rate on solar panels (% per year, rough)
inline constexpr double DUST_ACCUMULATION_RATE = 0.08;

/// Battery round-trip efficiency
inline constexpr double BATTERY_EFFICIENCY = 0.92;

/// Nuclear reactor specific power (W/kg, rough for surface reactor)
inline constexpr double NUCLEAR_SPECIFIC_POWER_W_PER_KG = 5.0;

} // namespace mars::power
