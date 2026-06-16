cmake_minimum_required(VERSION 3.20)
project(mars_sim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Production flags: warnings, optimize
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
endif()

# Core library
add_library(mars_foundation STATIC
    src/foundation/environment.cpp
)
target_include_directories(mars_foundation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Example executable
add_executable(foundation_demo examples/foundation_demo.cpp)
target_link_libraries(foundation_demo PRIVATE mars_foundation)

# Simple test executable (no external test framework for standalone)
add_executable(foundation_tests tests/foundation_tests.cpp)
target_link_libraries(foundation_tests PRIVATE mars_foundation)

# Install rules (production)
install(TARGETS mars_foundation foundation_demo foundation_tests
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

message(STATUS "Mars Simulator Foundation configured. Build with: cmake --build . --config Release")
