cmake_minimum_required(VERSION 3.10)
project(NewsAggregatorClient)

set(CMAKE_CXX_STANDARD 17)

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)

file(GLOB_RECURSE SOURCES
    "core/*.cpp"
    "services/*.cpp"
    "ui/*.cpp"
    "utils/*.cpp"
    "main.cpp"
)

include_directories(
    core/
    services/
    ui/
    models/
    utils/
)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(NewsAggregatorClient main.cpp ${SOURCES})

target_link_libraries(NewsAggregatorClient ${CURL_LIBRARIES})
