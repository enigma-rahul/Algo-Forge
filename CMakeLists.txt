cmake_minimum_required(VERSION 3.14)
project(AlgoForge VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings for student clarity & clean code
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party
)

# Core Algorithm Engine Library
set(ENGINE_SOURCES
    src/StepRecorder.cpp
    src/algorithms/sorting.cpp
    src/algorithms/searching.cpp
    src/algorithms/graph.cpp
    src/algorithms/tree.cpp
)

add_library(algo_forge_engine STATIC ${ENGINE_SOURCES})

# 1. Server Executable
add_executable(algo_forge_server
    src/main.cpp
    src/server.cpp
)

target_link_libraries(algo_forge_server PRIVATE
    algo_forge_engine
    pthread
)

# 2. Automated Test Suite Executable
add_executable(algo_tests
    tests/test_main.cpp
)

target_link_libraries(algo_tests PRIVATE
    algo_forge_engine
)

enable_testing()
add_test(NAME EngineTests COMMAND algo_tests)
