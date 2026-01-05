cmake_minimum_required(VERSION 3.20)

# ------------------------------------------------------------------------------
# Project Setup
# ------------------------------------------------------------------------------
project(drills
  LANGUAGES C CXX
  DESCRIPTION "Programming drills")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Default to Release if no build type is set:
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
#  set(CMAKE_BUILD_TYPE Release)
endif()

# Set a default parallel build level if the user hasn't specified one.
if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 16 CACHE STRING "Default parallel build level" FORCE)
endif()

# Some extra logging for the user:
message(STATUS "----------------------------------------")
message(STATUS "CMAKE_SYSTEM_NAME:     ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_BUILD_TYPE:      ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_GENERATOR:       ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_COMMAND:         ${CMAKE_COMMAND}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "----------------------------------------")

# ------------------------------------------------------------------------------
# Deps
# ------------------------------------------------------------------------------
set(FETCHCONTENT_QUIET OFF)
include(FetchContent)
include(ExternalProject)


# GTest, last one with C++14 (required by Google Benchmark)
FetchContent_Declare(
  GoogleTest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
  GIT_SHALLOW 1
)
FetchContent_MakeAvailable(GoogleTest)

# Google Benchmark
FetchContent_Declare(
  GoogleBenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.4
  GIT_SHALLOW 1
)

# Suppress building tests/docs/etc. for faster builds:
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_DOXYGEN OFF CACHE BOOL "" FORCE)
set(BENCHMARK_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_USE_BUNDLED_GTEST ON CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_LIBPFM OFF CACHE BOOL "" FORCE)
set(HAVE_STD_REGEX ON)

FetchContent_MakeAvailable(GoogleBenchmark)

# # FMT for logging, as `std::format` has limited functionality
# FetchContent_Declare(
#   VictorZverovichFMT
#   GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#   GIT_TAG 11.1.2
#   GIT_SHALLOW 1
# )
# FetchContent_MakeAvailable(VictorZverovichFMT)


# Release configuration: Remove Google Benchmark's built-in debug warnings
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_definitions(benchmark PRIVATE NDEBUG)
endif()

enable_testing()
file(GLOB TEST_SOURCES LIST_DIRECTORIES false 
  ${PROJECT_SOURCE_DIR}/two-pointers/*.cpp
  ${PROJECT_SOURCE_DIR}/multithreading/*.cpp
  ${PROJECT_SOURCE_DIR}/sorting/*.cpp
  ${PROJECT_SOURCE_DIR}/math/*.cpp
)

# Add source to this project's executable.
add_executable (drills "drills.cpp" ${TEST_SOURCES})

option(DRILLS_BUILD_TESTS "Build the test suite" ON)

# Debug configuration: Edit-and-Continue (/ZI) for MSVC
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(MSVC)
  target_compile_definitions(drills PRIVATE "/ZI")
  endif()
endif()


set_target_properties(drills PROPERTIES 
  POSITION_INDEPENDENT_CODE ON)

target_include_directories(drills PRIVATE "include")

target_link_libraries(drills
  PRIVATE
  GTest::gtest_main
  benchmark::benchmark
  # fmt::fmt
)

include(GoogleTest)
gtest_discover_tests(drills)
