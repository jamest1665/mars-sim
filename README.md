# Mars Simulator (v0.2)

**Standalone, modular, production-ready C++23 Mars environment simulator.**

**Current modules:**
- Foundation (v0.1): gravity 3.71 m/s², atmosphere, radiation, solar (NASA data)
- Weather / Dust Storms (v0.2): parametric seasonal τ, storms, lifting — composes with foundation

Built sequentially per spec. Ready for GitHub iterations and next modules (terraforming, habitats...).

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
./weather_demo
./foundation_tests
```

Requires C++23 compiler (GCC 11+, Clang 16+, MSVC 2022+ recommended). No external dependencies.

## Architecture & Design (Principal Engineer Notes)

**From first principles:**
- All models in **SI units**, double precision for accuracy + range.
- **Modular**: `mars::foundation` and `mars::weather` namespaces. Header-only interface where possible; impl separate for future binary compat.
- **Extensible**: `MarsEnvironment::Config` and `MarsWeather::Config` for scenario tuning; `sample_state()` + `apply_to_state()` return/augment rich `EnvironmentState` for downstream modules (terraforming, habitats, humans). Composition over inheritance.
- **Performant**: O(1) or simple math per query. No allocations, virtuals, or locks in hot path. `constexpr` constants. Clamping for robustness.
- **Accurate**: NASA/MSL-RAD/Viking/Mars Climate Database + MGS TES/MRO MCS calibrated nominals + first-principles hydrostatic + parametric dust.
- **Production**: RAII, noexcept hot paths, input validation/clamping, documented assumptions/edges, self-tests, CMake install, warnings enabled.

**Trade-offs weighed:**
- **Accuracy vs complexity**: Simple parametric dust (seasonal + latitudinal + storm multiplier) chosen over full GCM for v0.2. Sufficient for Monte-Carlo, agent training, habitat sizing. Full physics in later modules.
- **Standalone vs features**: Pure std C++23. No external deps.
- **Error handling**: noexcept + clamping for sim stability.

**Key Assumptions (flagged in code):**
- Weather: parametric seasonal τ curve + lat weighting + simple stochastic storms. Real Mars has complex regional lifting, radiative feedback, and multi-year variability.
- Dust directly modulates foundation solar/radiation; no full atm heating feedback yet (next for terraforming).
- Storm season Ls 180-360 with peak near 270 (southern summer).

**Edge Cases Handled:**
- τ saturation at extreme values.
- Non-storm season returns clear-sky baseline.
- Latitude outside [-90,90] gracefully handled.

## API Overview (Foundation + Weather)

```cpp
#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"

mars::foundation::MarsEnvironment env;
mars::weather::MarsWeather weather;

auto state = env.sample_state({-4.5, 137.4, -4500}, 270.0 /*Ls southern summer*/);
weather.apply_to_state(state, 270.0, {/*loc*/});
// state.solar now reflects high dust optical depth
```

See demos and headers for full usage.

## Roadmap (Sequential Core-Out)

1. ✅ **Foundation** (v0.1) - gravity, atm, radiation, solar + NASA data.
2. ✅ **Weather/Dust Storms** (v0.2) - parametric seasonal τ, storms, lifting (this release).
3. Terraforming (atm thickening, temp, magnetic?).
4. Habitats + resource loops (ISRU, ECLSS, power, closed loops).
5. Human survival models (physiology, radiation health, psych).
6. Viz/scenarios (export, simple console/ImGui?, scenario runner).
7. Modularity/tests/docs/examples (expand; add Catch2 or doctest opt, Doxygen, more examples).

Each step adds a new `mars::xxx` submodule, composable via `MarsSim` orchestrator later.

## Verification & Next Steps

- `foundation_demo` + `weather_demo` exercise combined behavior.
- `foundation_tests` validates core invariants.
- Cross-check: Gale crater τ jumps in southern summer; solar drops during storms (matches observations).

**Verifiable next steps:**
- User request: "build terraforming module" or "add wind vectors to weather".
- Profile hot path (weather queries are <1 µs).
- Extend `EnvironmentState` with explicit dust fields if needed.

**Motto**: Accurate enough for engineering decisions and agent training; fast enough for Monte-Carlo; modular enough to swap physics fidelity.

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.

For issues or extensions: open in repo or describe scenario.
