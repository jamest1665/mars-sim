# Mars Simulator - Foundation Module (v0.1)

**Standalone, modular, production-ready C++23 Mars environment simulator core.**

Built sequentially per spec: **Foundation first** (gravity 3.71 m/s², radiation, atmosphere + NASA data).

> Ready for GitHub iterations, feature branches, and sequential module additions (weather, terraforming, habitats...).

## Repository

- Clone: `git clone https://github.com/jamest1665/mars-sim.git`
- Issues / PRs welcome for refinements or next modules.
- Commit style: short, imperative, scoped (e.g. `feat(foundation): add hydrostatic atmosphere model`).

## Quick Start

```bash
git clone https://github.com/jamest1665/mars-sim.git
cd mars-sim
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
./foundation_demo
./foundation_tests
```

Requires C++23 compiler (GCC 11+, Clang 16+, MSVC 2022+ recommended). No external dependencies.

## Architecture & Design (Principal Engineer Notes)

**From first principles:**
- All models in **SI units**, double precision for accuracy + range.
- **Modular**: `mars::foundation` namespace. Header-only interface where possible; impl separate for future binary compat.
- **Extensible**: `MarsEnvironment::Config` for scenario tuning; `sample_state()` returns rich `EnvironmentState` struct for downstream modules (weather, habitats, humans). No inheritance lock-in yet — compose or wrap.
- **Performant**: O(1) or simple math per query. No allocations, virtuals, or locks in hot path. `constexpr` constants. Clamping for robustness.
- **Accurate**: NASA/MSL-RAD/Viking/Mars Climate Database calibrated nominals + first-principles hydrostatic + Beer-Lambert solar. 
- **Production**: RAII, noexcept hot paths, input validation/clamping, documented assumptions/edges, self-tests, CMake install, warnings enabled.

**Trade-offs weighed:**
- **Accuracy vs complexity**: Simple parametric (exponential atm, linear rad shielding, geometric solar) chosen over full GCM/GEANT4 for foundation. Sufficient for habitat sim, agent training, scenario exploration. Later modules (weather, terraforming) can inject refined fields.
- **Standalone vs features**: Pure std C++23. No Boost, no Eigen (yet), no external viz. Export CSV/JSON from states for Python/Matplotlib later. Keeps build trivial and portable.
- **Modules (C++20/23)**: Deferred. Compiler fragmentation still high for production cross-platform. Traditional headers + CMake is reliable and familiar.
- **Error handling**: noexcept + clamping for sim stability (no crashes on bad alt). Future: `std::expected` for strict callers.
- **Stochastic**: None in foundation (deterministic). Weather/dust will add RNG.

**Key Assumptions (flagged everywhere in code):**
- Gravity: constant 3.71 m/s² (actual mean 3.721 ±0.03; local anomalies ignored v1).
- Atmosphere: isothermal hydrostatic exponential (good <15-20 km; real has diurnal/seasonal waves, dust loading). Fixed CO2 R.
- Radiation: GCR dominant, linear pressure correction (empirical from RAD ~0.15 uGy/d/Pa). Ignores full solar cycle, SPE spectra, albedo neutrons, regolith. SEP is binary flag (user-driven).
- Solar: TOA + simple direct attenuation + rough diffuse. No multiple scattering, no surface albedo feedback yet. Zenith approx (no full ephemeris).
- No magnetic field, no topography shadowing (later), no time evolution in foundation (static snapshot; timestep in higher modules).

**Edge Cases Handled:**
- Altitude out of [-10km, +25km] → clamped.
- Pressure <10 Pa or >1200 Pa → model floors/ceils.
- Zenith >89° → direct beam = 0.
- T<=0 → density=0 guard.
- Extreme ls/time → stable trig.

**Data Sources (double-checked):**
- Gravity: NASA Glenn, Wikipedia (areoid), MGS/MRO gravity models.
- Atmosphere: NASA Mars Facts, Viking lander, MRO/Mars Climate Sounder. Avg P 610 Pa, T~210K, scale ~11km.
- Radiation: MSL-RAD (Hassler, Zeitlin, Guo et al. 2013-2021 papers). ~0.64-0.7 mSv/d GCR surface, varies solar cycle/season/pressure.
- Solar: NASA/ASTM E-490, Appplebaum 1989 Mars solar radiation model. ~590 W/m² mean TOA.

## API Overview

```cpp
#include "mars/foundation/environment.hpp"

mars::foundation::MarsEnvironment env({.base_pressure_pa = 700.0}); // custom
auto state = env.sample_state({-4.5, 137.4, -4500}, 120.0 /*Ls*/, 0.6 /*time*/);
double p = env.pressure_at_altitude(5000.0);
```

See `include/mars/foundation/*.hpp` and `foundation_demo.cpp` for full usage.

## Roadmap (Sequential Core-Out)

1. ✅ **Foundation** (this) - gravity, atm, radiation, solar + NASA data.
2. Weather/dust storms (dynamic, tau evolution, global circulation hints).
3. Terraforming (atm thickening, temp, magnetic?).
4. Habitats + resource loops (ISRU, ECLSS, power, closed loops).
5. Human survival models (physiology, radiation health, psych).
6. Viz/scenarios (export, simple console/ImGui?, scenario runner).
7. Modularity/tests/docs/examples (expand; add Catch2 or doctest opt, Doxygen, more examples).

Each step adds a new `mars::xxx` submodule, composable via `MarsSim` orchestrator later.

## Verification & Next Steps

- `foundation_tests` validates key invariants.
- `foundation_demo` exercises all paths + prints realistic numbers (Gale ~610Pa, ~0.6 mSv/d, etc.).
- Cross-check: Curiosity RAD data matches order-of-magnitude.

**Verifiable next steps (after this foundation):**
- User request: "build weather module" or "add dust storm dynamics".
- Compile/run on your target (Linux/Win/Mac).
- Profile hot path if integrating in loop (should be <1us/query).
- Extend Config for solar min/max or custom tau.
- Add CSV export helper for viz in step 6.

**Motto**: Accurate enough for engineering decisions and agent training; fast enough for Monte-Carlo; modular enough to swap physics fidelity.

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.

For issues or extensions: open in repo or describe scenario.
