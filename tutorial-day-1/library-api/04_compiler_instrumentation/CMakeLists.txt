cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(04_compiler_instrumentation LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# find timemory package with compiler-instrument component
#
# <insert code here>
#

add_executable(compiler_instrumentation)
target_sources(compiler_instrumentation PRIVATE example.cpp)

# link to timemory target
#
# <insert code here>
