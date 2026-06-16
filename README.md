# Mars Simulator (v1.1)

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
- **Campaign Simulator** (v1.1) — multi-year colony growth, Earth supply events, dynamic agents, rolling reliability

**New in v1.1**: Full Campaign Simulator turns the system into a real long-term mission planning engine.

## Quick Start

```bash
git clone https://github.com/jamest1665/mars-sim.git
cd mars-sim
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
./campaign_sim
```

Failure is mandatory. Quitting is not. — SkyForge Dynamics style.
