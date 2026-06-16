# Mars Simulator (v0.6)

**Standalone, modular, production-ready C++23 Mars environment simulator.**

**Current modules:**
- Foundation (v0.1)
- Weather / Dust Storms (v0.2)
- Terraforming (v0.3)
- Habitats + Resource Loops (v0.4)
- Human Survival Models (v0.5)
- Visualization & Scenario Runner (v0.6) — Monte Carlo, CSV export, what-if analysis

Full end-to-end pipeline from planetary physics to crew health + scenario exploration.

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
./viz_demo
./foundation_tests
```

Requires C++23 compiler (GCC 11+, Clang 16+, MSVC 2022+ recommended). No external dependencies.

## Architecture & Design (Principal Engineer Notes)

**From first principles:**
- All models in **SI units**, double precision.
- **Modular** composition via rich `EnvironmentState` passed between layers.
- Full pipeline now exists from planetary physics → weather → terraforming → habitats → human health → scenario/Monte Carlo exploration.

**v0.6 Achievement**
Complete physics-to-scenario capability. Users can now run Monte Carlo studies and export data for analysis.

## API Overview (v0.6)

```cpp
#include "mars/viz/viz.hpp"

mars::viz::ScenarioRunner runner;
auto results = runner.run_monte_carlo(100, 3.0);
runner.export_monte_carlo_summary(results, "summary.csv");
runner.export_to_csv(results[0], "detailed_run.csv");
```

See `viz_demo` for concrete usage.

## Roadmap (Sequential Core-Out)

1. ✅ **Foundation** (v0.1)
2. ✅ **Weather/Dust Storms** (v0.2)
3. ✅ **Terraforming** (v0.3)
4. ✅ **Habitats + Resource Loops** (v0.4)
5. ✅ **Human Survival Models** (v0.5)
6. ✅ **Visualization & Scenario Runner** (v0.6) — this release
7. Modularity, testing, documentation & examples expansion
8+. Autonomous agents, logistics/economics, full mission orchestrator

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.
