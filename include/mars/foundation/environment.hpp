#pragma once

/**
 * @file environment.hpp
 * @brief MarsEnvironment core class: provides accurate, performant access to
 *        foundational Mars physical environment (gravity, atmosphere, radiation,
 *        solar). Designed for modularity, extensibility, and production use.
 *
 * Usage:
 *   mars::foundation::MarsEnvironment env; // defaults to nominal
 *   auto state = env.sample_state(latlonalt, ls, time);
 *   double g = env.gravity();
 *   double p = env.pressure_at_altitude(alt);
 *
 * Extensibility: subclass or compose with weather/terrain models later.
 * All hot paths inline/constexpr-friendly where feasible. Double precision.
 *
 * @see constants.hpp, types.hpp for data and assumptions.
 */

#include "constants.hpp"
#include "types.hpp"

#include <optional>
#include <string_view>

namespace mars::foundation {

class MarsEnvironment {
public:
    /// Configuration for model fidelity and scenario
    struct Config {
        double gravity_mps2 = GRAVITY_MARS;
        double base_pressure_pa = ATM_PRESSURE_SURFACE_PA;
        double base_temp_k = ATM_TEMPERATURE_SURFACE_K;
        double scale_height_m = ATM_SCALE_HEIGHT_M;
        double base_gcr_dose_msv_d = GCR_DOSE_EQUIV_RATE_MSV_D;
        double pressure_shielding_coeff = RAD_PRESSURE_SHIELDING_COEFF;
        double base_tau = DUST_TAU_CLEAR;
        bool enable_variations = true; // seasonal/diurnal/alt for foundation
        // Future: model selection enums, stochastic seed, etc.
    };

    MarsEnvironment();
    explicit MarsEnvironment(Config cfg);
    ~MarsEnvironment() = default;

    // Non-copyable if needed, but default ok for now (stateless-ish)
    MarsEnvironment(const MarsEnvironment&) = default;
    MarsEnvironment& operator=(const MarsEnvironment&) = default;

    /// Core accessors - O(1) or simple math, performant
    [[nodiscard]] double gravity() const noexcept { return m_cfg.gravity_mps2; }

    /// Simple hydrostatic pressure at altitude (exponential model)
    /// Assumption: isothermal, constant g, ideal gas, fixed composition.
    /// Edge: alt <0 or >max -> clamped or extrapolated with warning flag.
    [[nodiscard]] double pressure_at_altitude(double altitude_m) const noexcept;

    /// Temperature at altitude (simple lapse or constant for v1)
    [[nodiscard]] double temperature_at_altitude(double altitude_m) const noexcept;

    /// Density via ideal gas (CO2 dominant R)
    [[nodiscard]] double density_at(double altitude_m) const noexcept;

    /// Full atmosphere state at alt
    [[nodiscard]] AtmosphereState atmosphere_at(double altitude_m) const noexcept;

    /// Radiation: GCR dose equiv adjusted for pressure (shielding) + optional SEP
    /// Returns mSv/day. SEP flag for high transient (user must trigger).
    [[nodiscard]] RadiationState radiation_at(const LatLonAlt& loc,
                                              double pressure_pa = -1.0,
                                              bool sep_active = false) const noexcept;

    /// Solar TOA and surface estimate (simple Beer-Lambert direct + rough diffuse)
    /// ls_deg: solar longitude (season), lat for zenith approx, tau override.
    /// Returns global horizontal approx. No full radiative transfer.
    [[nodiscard]] SolarState solar_at(const LatLonAlt& loc,
                                      double ls_deg,
                                      double local_time_frac = 0.5,
                                      std::optional<double> tau_override = std::nullopt) const noexcept;

    /// Sample complete environment snapshot (foundation for scenarios)
    [[nodiscard]] EnvironmentState sample_state(const LatLonAlt& loc,
                                                double ls_deg = 0.0,
                                                double local_time_frac = 0.5,
                                                bool sep_active = false) const noexcept;

    /// Update config (e.g. for different solar cycle or base conditions)
    void set_config(const Config& cfg) noexcept { m_cfg = cfg; }
    [[nodiscard]] const Config& config() const noexcept { return m_cfg; }

    /// Validation / diagnostics
    [[nodiscard]] bool validate_location(const LatLonAlt& loc) const noexcept;
    [[nodiscard]] std::string_view model_description() const noexcept {
        return "Mars foundation v0.1: hydrostatic atm + parametric rad/solar (NASA calibrated)";
    }

private:
    Config m_cfg;

    // Internal helpers (non-virtual for perf)
    [[nodiscard]] double compute_zenith_angle(const LatLonAlt& loc, double ls_deg, double time_frac) const noexcept;
    [[nodiscard]] double compute_direct_attenuation(double tau, double zenith_rad) const noexcept;
};

} // namespace mars::foundation
