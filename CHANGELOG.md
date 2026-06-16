# Changelog

All notable changes to the Mars Simulator will be documented here.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
