# Changelog

All notable changes to the Mars Simulator will be documented here.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
