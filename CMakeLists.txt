cmake_minimum_required(VERSION 3.20)
project(SatelliteGroundStation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main
    src/main.cpp
    src/CsvHandler.cpp
)

target_include_directories(main PRIVATE include)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
