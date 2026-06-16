cmake_minimum_required(VERSION 3.20)
project(mars_sim VERSION 0.7.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Production flags: warnings, optimize
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
endif()

# Core libraries
add_library(mars_foundation STATIC
    src/foundation/environment.cpp
)
target_include_directories(mars_foundation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(mars_weather STATIC
    src/weather/weather.cpp
)
target_include_directories(mars_weather PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mars_weather PUBLIC mars_foundation)

add_library(mars_terraforming STATIC
    src/terraforming/terraforming.cpp
)
target_include_directories(mars_terraforming PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mars_terraforming PUBLIC mars_foundation mars_weather)

add_library(mars_habitats STATIC
    src/habitats/habitats.cpp
)
target_include_directories(mars_habitats PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mars_habitats PUBLIC mars_foundation mars_weather mars_terraforming)

add_library(mars_humans STATIC
    src/humans/humans.cpp
)
target_include_directories(mars_humans PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mars_humans PUBLIC mars_foundation mars_weather mars_terraforming mars_habitats)

add_library(mars_viz STATIC
    src/viz/viz.cpp
)
target_include_directories(mars_viz PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mars_viz PUBLIC mars_foundation mars_weather mars_terraforming mars_habitats mars_humans)

# Example executables
add_executable(foundation_demo examples/foundation_demo.cpp)
target_link_libraries(foundation_demo PRIVATE mars_foundation mars_weather mars_terraforming mars_habitats mars_humans)

add_executable(weather_demo examples/weather_demo.cpp)
target_link_libraries(weather_demo PRIVATE mars_foundation mars_weather)

add_executable(terraforming_demo examples/terraforming_demo.cpp)
target_link_libraries(terraforming_demo PRIVATE mars_foundation mars_weather mars_terraforming)

add_executable(habitats_demo examples/habitats_demo.cpp)
target_link_libraries(habitats_demo PRIVATE mars_foundation mars_weather mars_terraforming mars_habitats)

add_executable(humans_demo examples/humans_demo.cpp)
target_link_libraries(humans_demo PRIVATE mars_foundation mars_weather mars_terraforming mars_habitats mars_humans)

add_executable(viz_demo examples/viz_demo.cpp)
target_link_libraries(viz_demo PRIVATE mars_foundation mars_weather mars_terraforming mars_habitats mars_humans mars_viz)

# Simple test executable (no external test framework for standalone)
add_executable(foundation_tests tests/foundation_tests.cpp)
target_link_libraries(foundation_tests PRIVATE mars_foundation mars_weather mars_terraforming mars_habitats mars_humans mars_viz)

add_executable(integration_tests tests/integration_tests.cpp)
target_link_libraries(integration_tests PRIVATE mars_foundation mars_weather mars_terraforming mars_habitats mars_humans mars_viz)

# Install rules (production)
install(TARGETS mars_foundation mars_weather mars_terraforming mars_habitats mars_humans mars_viz foundation_demo weather_demo terraforming_demo habitats_demo humans_demo viz_demo foundation_tests integration_tests
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

message(STATUS "Mars Simulator v0.7 configured (testing + CI improvements). Build with: cmake --build . --config Release")
