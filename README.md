# Mars Simulator (v0.9)

**Standalone, modular, production-ready C++23 Mars environment simulator.**

**Current modules:**
- Foundation (v0.1)
- Weather / Dust Storms (v0.2)
- Terraforming (v0.3)
- Habitats + Resource Loops (v0.4)
- Human Survival Models (v0.5)
- Visualization & Scenario Runner (v0.6)
- Testing, CI & Documentation improvements (v0.7)
- Power Systems (v0.8)
- Closed-Loop Agent Control + Full Mission Orchestrator (v0.9)
- Surface Operations & Mobility (v0.8)
- Reliability & Failure Modeling (v0.9)
- Reference Missions (v0.9)

Full end-to-end pipeline from planetary physics → crew health → autonomous agents → surface operations + reliability.

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
./reference_mission
```

Requires C++23 compiler (GCC 11+, Clang 16+, MSVC 2022+ recommended). No external dependencies.

## Architecture & Design (Principal Engineer Notes)

**From first principles:**
- All models in **SI units**, double precision.
- **Modular** composition via rich `EnvironmentState` passed between layers.
- The simulator now supports closed-loop agent decision making inside full mission simulations, with reliability overlays.

**v0.9 Status**
Agents can now influence power and operations in real time. Reliability and reference mission capability added.

## Roadmap (Sequential Core-Out)

1. ✅ **Foundation** (v0.1)
2. ✅ **Weather/Dust Storms** (v0.2)
3. ✅ **Terraforming** (v0.3)
4. ✅ **Habitats + Resource Loops** (v0.4)
5. ✅ **Human Survival Models** (v0.5)
6. ✅ **Visualization & Scenario Runner** (v0.6)
7. ✅ **Testing + CI** (v0.7)
8. ✅ **Power + Agent Control + Surface Ops** (v0.8)
9. ✅ **Agent Integration + Reliability + Reference Missions** (v0.9)

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.
