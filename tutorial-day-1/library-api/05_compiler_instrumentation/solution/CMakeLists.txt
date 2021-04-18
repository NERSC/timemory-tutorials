cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(04_compiler_instrumentation LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# find timemory package with compiler-instrument component
#
find_package(timemory REQUIRED
    COMPONENTS compiler-instrument
    OPTIONAL_COMPONENTS cxx)
#

add_executable(compiler_instrumentation)
target_sources(compiler_instrumentation PRIVATE example.cpp)

# link to timemory target
#
target_link_libraries(compiler_instrumentation PRIVATE
    timemory::timemory-compiler-instrument)
