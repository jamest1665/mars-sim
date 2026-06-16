# Mars Simulator (v1.0)

**Standalone, modular, production-ready C++23 Mars environment simulator.**

**Current modules:**
- Foundation (v0.1)
- Weather / Dust Storms (v0.2)
- Terraforming (v0.3)
- Habitats + Resource Loops (v0.4)
- Human Survival Models (v0.5)
- Visualization & Scenario Runner (v0.6)
- Testing, CI & Documentation (v0.7)
- Power Systems (v0.8)
- Closed-Loop Agent Control + Multiple Strategies (v1.0)
- Surface Operations & Mobility (v0.8)
- Reliability, Failure Modes & Maintenance (v1.0)
- Reference Mission Library & Validation (v1.0)

Full physics-to-crew + autonomous agents + reliability modeling. Version 1.0 complete.

## Quick Start

```bash
git clone https://github.com/jamest1665/mars-sim.git
cd mars-sim
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
./reference_mission
./mission_profiles
./validation
```

## Roadmap Achieved

All core sequential modules complete through v1.0, including closed-loop agent control, reliability, and scenario library.

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.
