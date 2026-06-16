# Mars Simulator (v0.4)

**Standalone, modular, production-ready C++23 Mars environment simulator.**

**Current modules:**
- Foundation (v0.1): gravity 3.71 m/s², atmosphere, radiation, solar (NASA data)
- Weather / Dust Storms (v0.2): parametric seasonal τ, storms, lifting
- Terraforming (v0.3): atmosphere thickening, temperature, radiation shielding
- Habitats + Resource Loops (v0.4): ISRU, ECLSS, power, habitat sizing — composes with prior modules

Built sequentially per spec. Ready for GitHub iterations and next modules (human survival models...).

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
./habitats_demo
./foundation_tests
```

Requires C++23 compiler (GCC 11+, Clang 16+, MSVC 2022+ recommended). No external dependencies.

## Architecture & Design (Principal Engineer Notes)

**From first principles:**
- All models in **SI units**, double precision.
- **Modular** composition via rich `EnvironmentState` passed between layers.
- Each new module adds fidelity to the environmental state that downstream modules (habitats, humans) consume.

**v0.4 Stack Summary**
Foundation (physics) → Weather (dust/storms) → Terraforming (atm modification) → Habitats (ISRU/ECLSS/power/sizing)

## API Overview (v0.4 Full Stack)

```cpp
#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"
#include "mars/terraforming/terraforming.hpp"
#include "mars/habitats/habitats.hpp"

MarsEnvironment env;
MarsWeather weather;
MarsTerraforming terra;
MarsHabitats habitats;

auto state = env.sample_state(loc, ls);
weather.apply_to_state(state, ls, loc);
terra.apply_to_state(state, years, loc);
auto resources = habitats.calculate_resources(state, years);
// resources now contains sustainable O2, water, power, and habitat mass
```

See the four demos for end-to-end usage.

## Roadmap (Sequential Core-Out)

1. ✅ **Foundation** (v0.1)
2. ✅ **Weather/Dust Storms** (v0.2)
3. ✅ **Terraforming** (v0.3)
4. ✅ **Habitats + Resource Loops** (v0.4) — this release
5. Human survival models
6. Viz/scenarios
7. Modularity/tests/docs/examples

**Next logical step**: Human survival models (radiation health, low-g physiology, nutrition, psychology).

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.
