cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(basic LANGUAGES C CXX)

include(GNUInstallDirs)

option(USE_MPI "Enable MPI" ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build type" FORCE)
endif()

find_package(timemory REQUIRED COMPONENTS cxx
    OPTIONAL_COMPONENTS compile-options shared)
target_compile_definitions(timemory INTERFACE USE_TIMEMORY)

if(USE_MPI)
    find_package(MPI REQUIRED)
    list(APPEND _MPI MPI::MPI_CXX)
    target_compile_definitions(MPI::MPI_CXX INTERFACE USE_MPI)
endif()

find_package(Threads)

add_executable(${PROJECT_NAME}           basic.cpp           basic.hpp)
add_executable(${PROJECT_NAME}.timemory  basic-timemory.cpp  basic.hpp)

target_link_libraries(${PROJECT_NAME}          ${_MPI} Threads::Threads)
target_link_libraries(${PROJECT_NAME}.timemory ${_MPI} Threads::Threads timemory)

install(TARGETS ${PROJECT_NAME}          DESTINATION bin)
install(TARGETS ${PROJECT_NAME}.timemory DESTINATION bin)
