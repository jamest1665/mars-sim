cmake_minimum_required(VERSION 3.20)
project(mars_sim VERSION 0.5.0 LANGUAGES CXX)

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

# Simple test executable (no external test framework for standalone)
add_executable(foundation_tests tests/foundation_tests.cpp)
target_link_libraries(foundation_tests PRIVATE mars_foundation mars_weather mars_terraforming mars_habitats mars_humans)

# Install rules (production)
install(TARGETS mars_foundation mars_weather mars_terraforming mars_habitats mars_humans foundation_demo weather_demo terraforming_demo habitats_demo humans_demo foundation_tests
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

message(STATUS "Mars Simulator v0.5 configured (full stack through humans). Build with: cmake --build . --config Release")
