cmake_minimum_required(VERSION 3.20)
project(SatelliteGroundStation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(main
    src/main.cpp
    src/CsvHandler.cpp
    src/N2yoClient.cpp
)

target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
