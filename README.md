# Mars Simulator (v0.3)

**Standalone, modular, production-ready C++23 Mars environment simulator.**

**Current modules:**
- Foundation (v0.1): gravity 3.71 m/s², atmosphere, radiation, solar (NASA data)
- Weather / Dust Storms (v0.2): parametric seasonal τ, storms, lifting
- Terraforming (v0.3): atmosphere thickening, temperature, radiation shielding — composes with prior modules

Built sequentially per spec. Ready for GitHub iterations and next modules (habitats, humans...).

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
./terraforming_demo
./foundation_tests
```

Requires C++23 compiler (GCC 11+, Clang 16+, MSVC 2022+ recommended). No external dependencies.

## Architecture & Design (Principal Engineer Notes)

**From first principles:**
- All models in **SI units**, double precision for accuracy + range.
- **Modular**: `mars::foundation`, `mars::weather`, `mars::terraforming` namespaces. Composition via rich `EnvironmentState`.
- **Extensible**: Each module exposes `Config` and `apply_to_state()` style methods so later layers (habitats, humans) receive progressively richer environmental input.
- **Performant**: All hot paths O(1) or simple math. No allocations or virtuals in queries.
- **Accurate**: NASA-calibrated baselines + first-principles models (hydrostatic, Beer-Lambert, energy balance, column-depth radiation). Parametric where full physics would be premature.
- **Production**: noexcept, documented assumptions/edges, self-tests, CMake, warnings, clean history.

**Current Stack (v0.3)**
Foundation (static physics) → Weather (dynamic dust/storms) → Terraforming (deliberate atm modification) → ready for Habitats & Human models.

## API Overview (Full v0.3 Stack)

```cpp
#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"
#include "mars/terraforming/terraforming.hpp"

MarsEnvironment env;
MarsWeather weather;
MarsTerraforming terra;

auto state = env.sample_state(loc, ls);
weather.apply_to_state(state, ls, loc);
terra.apply_to_state(state, years, loc);
// state now reflects foundation + weather + terraformed conditions
```

See the three demos for concrete usage.

## Roadmap (Sequential Core-Out)

1. ✅ **Foundation** (v0.1)
2. ✅ **Weather/Dust Storms** (v0.2)
3. ✅ **Terraforming** (v0.3) — this release
4. Habitats + resource loops
5. Human survival models
6. Viz/scenarios
7. Modularity/tests/docs/examples

## Verification
- `terraforming_demo` shows realistic multi-decade pressure rise, warming, and GCR reduction.
- All modules compose without breaking changes to earlier layers.

**Next logical step**: Habitats + resource loops (ISRU, ECLSS, power, closed loops) once we have a terraformed environmental state.

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.
