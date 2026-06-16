/**
 * @file foundation_tests.cpp
 * @brief Basic self-contained tests for foundation correctness and robustness.
 *        Run: ./foundation_tests
 *        Exit 0 = pass. Production: expand with property tests, CI.
 */

#include "mars/foundation/environment.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace mars::foundation;

int main() {
    MarsEnvironment env;

    // Test 1: gravity exact
    assert(std::abs(env.gravity() - 3.71) < 1e-9);
    std::cout << "[PASS] Gravity baseline\n";

    // Test 2: pressure at surface ~610 Pa
    double p0 = env.pressure_at_altitude(0.0);
    assert(std::abs(p0 - 610.0) < 5.0); // tolerance for model
    std::cout << "[PASS] Surface pressure ~610 Pa (got " << p0 << ")\n";

    // Test 3: pressure decreases with alt (hydrostatic)
    double p_high = env.pressure_at_altitude(10000.0);
    assert(p_high < p0 * 0.5);
    std::cout << "[PASS] Pressure drops with altitude\n";

    // Test 4: density positive, reasonable
    double rho = env.density_at(0.0);
    assert(rho > 0.01 && rho < 0.03);
    std::cout << "[PASS] Density reasonable\n";

    // Test 5: radiation positive, SEP increases
    LatLonAlt loc{0,0,0};
    auto rad = env.radiation_at(loc);
    auto rad_sep = env.radiation_at(loc, -1.0, true);
    assert(rad.gcr_dose_equiv_msv_per_day > 0.1);
    assert(rad_sep.total_dose_equiv_msv_per_day > rad.gcr_dose_equiv_msv_per_day);
    std::cout << "[PASS] Radiation model + SEP boost\n";

    // Test 6: solar TOA ~590, varies with ls a bit
    auto sol = env.solar_at(loc, 0.0, 0.5);
    assert(sol.toa_irradiance_wm2 > 500.0 && sol.toa_irradiance_wm2 < 700.0);
    std::cout << "[PASS] Solar TOA in range\n";

    // Test 7: night side solar ~0 direct
    auto sol_night = env.solar_at(loc, 0.0, 0.0); // midnight
    assert(sol_night.direct_beam_wm2 < 1.0);
    std::cout << "[PASS] Night side zero direct beam\n";

    // Test 8: validate clamps extremes
    LatLonAlt bad{95.0, 0, 30000.0};
    assert(!env.validate_location(bad));
    auto state = env.sample_state(bad);
    assert(state.location.altitude_m <= ALTITUDE_MAX_M);
    std::cout << "[PASS] Location validation + clamping\n";

    std::cout << "\nAll foundation tests PASSED.\n";
    return 0;
}
