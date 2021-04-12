cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(01_compiler_instrumentation LANGUAGES CXX)

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
#

add_executable(compiler_instrumentation_without_timemory)
target_sources(compiler_instrumentation_without_timemory          PRIVATE example_with_timemory.cpp)
target_link_libraries(compiler_instrumentation_without_timemory   PRIVATE timemory::timemory-compiler-instrument)

add_executable(compiler_instrumentation_with_timemory)
target_sources(compiler_instrumentation_with_timemory             PRIVATE example_with_timemory.cpp)
target_compile_definitions(compiler_instrumentation_with_timemory PRIVATE USE_TIMEMORY)
target_link_libraries(compiler_instrumentation_with_timemory      PRIVATE timemory::timemory)

enable_testing()
add_test(
    NAME              compiler-instrumentation
    COMMAND           $<TARGET_FILE:compiler_instrumentation>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:compiler_instrumentation>)

add_test(
    NAME              compiler-instrumentation-with-timemory
    COMMAND           $<TARGET_FILE:compiler_instrumentation_with_timemory>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:compiler_instrumentation_with_timemory>)

add_test(
    NAME              compiler-instrumentation-without-timemory
    COMMAND           $<TARGET_FILE:compiler_instrumentation_without_timemory>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:compiler_instrumentation_without_timemory>)
