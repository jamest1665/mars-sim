# Changelog

All notable changes to the Mars Simulator will be documented here.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.6.0] - 2026-06-16

### Added
- Visualization & Scenario Runner module (`mars::viz`): Time-series data collection from the full stack, Monte Carlo scenario runner with parameter variation, CSV export for external analysis (pandas/matplotlib), and summary statistics.
- `ScenarioRunner` with `run_single_scenario()`, `run_monte_carlo()`, and export utilities.
- `viz_demo` that runs Monte Carlo and exports results.
- Full v0.6 stack with scenario exploration capability.

### Notes
- Enables rapid what-if analysis and Monte Carlo studies of mission success probability under different terraforming/habitat assumptions.
- CSV output makes it trivial to generate plots and statistics externally.
- Sets the stage for more advanced optimization and agent-driven scenario generation.

## [0.5.0] - 2026-06-16

### Added
- Human Survival Models module (`mars::humans`): radiation health (cumulative dose, cancer risk, ARS), low-g physiology (bone/muscle loss, cardiovascular), nutrition status from habitat resources, psychological wellbeing.
- `MarsHumans` class with `update_health()` that produces comprehensive crew health metrics from the full upstream stack.
- `humans_demo` showing 3-year mission health trajectory with good terraforming + habitats.
- Full v0.5 end-to-end stack.

### Notes
- With aggressive terraforming and sustainable habitats, crew health remains within acceptable limits for a 3-year mission.
- Models are first-order parametric. Detailed medical simulation and countermeasures optimization planned for later.
- Project now has a complete physics-to-human pipeline.

## [0.4.0] - 2026-06-16

### Added
- Habitats + Resource Loops module (`mars::habitats`): ISRU (Sabatier O2 production), ECLSS water recycling, power generation (solar + nuclear option), habitat mass and shielding sizing based on radiation and pressure.
- `MarsHabitats` class with `calculate_resources()` that produces sustainable closed-loop metrics from the terraformed EnvironmentState.
- `habitats_demo` showing how terraforming progress enables sustainable 6-crew habitats.
- Full v0.4 stack integration.

### Notes
- Habitat mass and power requirements decrease significantly as terraforming advances (higher pressure, lower radiation).
- Closed-loop sustainability becomes achievable around year 50-100 in aggressive scenarios.
- Ready for Human survival models (radiation health, low-g effects).

## [0.3.0] - 2026-06-16

### Added
- Terraforming module (`mars::terraforming`): parametric atmosphere thickening, temperature response via greenhouse effect, radiation shielding improvement with column depth, basic ISRU energy cost proxy.
- `MarsTerraforming` class with `Config` and `apply_to_state()` that composes cleanly with foundation + weather `EnvironmentState`.
- `terraforming_demo` showing 100-year progression at a representative site (pressure, temp, GCR dose, solar).
- Full v0.3 stack demo and updated build/docs.

## [0.2.0] - 2026-06-16

### Added
- Weather / Dust Storms module (`mars::weather`): parametric seasonal optical depth (Ls-driven), regional latitudinal weighting, stochastic/threshold storm events, lifting potential proxy.
- `MarsWeather` class with `Config`, `optical_depth()`, `sample_dust()`, `apply_to_state()` for seamless composition with foundation `EnvironmentState`.
- `weather_demo` showing combined foundation + weather over a full Mars year at Gale crater.
- Updated CMake, demos, and docs for v0.2.

### Notes
- Dust τ now dynamically affects solar attenuation and radiation in foundation queries.
- Calibrated to MGS TES, MRO MCS, and published dust climatology. Good first-order for scenarios; full GCM planned for later.
- Ready for terraforming (atm heating feedback) and habitat power/ISRU modules.

## [0.1.0] - 2026-06-16

### Added
- Foundation module: gravity (3.71 m/s² baseline), hydrostatic atmosphere model, NASA-calibrated radiation (MSL-RAD GCR), solar insolation with dust optical depth.
- `MarsEnvironment` class with `Config`, `sample_state()`, and query methods.
- Full SI units, double precision, noexcept hot paths, input clamping.
- Self-tests, demo executable, CMake build, MIT license.
- Comprehensive inline documentation of assumptions, edges, and NASA sources.

### Notes
- First core-out deliverable. Ready for iterative extension (weather/dust, terraforming, etc.).
- All models validated against NASA/MSL-RAD/Viking data order-of-magnitude.
