# Mars Simulator (v0.5)

**Standalone, modular, production-ready C++23 Mars environment simulator.**

**Current modules:**
- Foundation (v0.1): gravity 3.71 m/s², atmosphere, radiation, solar (NASA data)
- Weather / Dust Storms (v0.2): parametric seasonal τ, storms, lifting
- Terraforming (v0.3): atmosphere thickening, temperature, radiation shielding
- Habitats + Resource Loops (v0.4): ISRU, ECLSS, power, habitat sizing
- Human Survival Models (v0.5): radiation health, low-g physiology, nutrition, psychology — full pipeline

Built sequentially per spec. Project complete through human-level metrics.

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
./humans_demo
./foundation_tests
```

Requires C++23 compiler (GCC 11+, Clang 16+, MSVC 2022+ recommended). No external dependencies.

## Architecture & Design (Principal Engineer Notes)

**From first principles:**
- All models in **SI units**, double precision.
- **Modular** composition via rich `EnvironmentState` passed between layers.
- Full pipeline now exists from planetary physics → weather → terraforming → habitats → human health metrics.

**v0.5 Achievement**
Complete end-to-end Mars mission simulation capability (environment → sustainable habitat → crew health over years).

## API Overview (v0.5 Full Pipeline)

```cpp
#include "mars/foundation/environment.hpp"
#include "mars/weather/weather.hpp"
#include "mars/terraforming/terraforming.hpp"
#include "mars/habitats/habitats.hpp"
#include "mars/humans/humans.hpp"

MarsEnvironment env;
MarsWeather weather;
MarsTerraforming terra;
MarsHabitats habitats;
MarsHumans humans;

auto state = env.sample_state(loc, ls);
weather.apply_to_state(state, ls, loc);
terra.apply_to_state(state, years, loc);
auto resources = habitats.calculate_resources(state, years);
auto health = humans.update_health(state, resources, years);
// health now contains bone/muscle loss, radiation risk, nutrition, psych metrics
```

See the five demos for complete usage.

## Roadmap (Sequential Core-Out)

1. ✅ **Foundation** (v0.1)
2. ✅ **Weather/Dust Storms** (v0.2)
3. ✅ **Terraforming** (v0.3)
4. ✅ **Habitats + Resource Loops** (v0.4)
5. ✅ **Human Survival Models** (v0.5) — this release
6. Viz/scenarios
7. Modularity/tests/docs/examples

**Project Status**: Full physics-to-human pipeline complete. Ready for visualization, scenario exploration, or agent training use cases.

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.
