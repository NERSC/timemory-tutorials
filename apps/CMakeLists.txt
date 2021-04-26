cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# default install directory should be parent directory of source
get_filename_component(INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR} PATH)
set(CMAKE_C_FLAGS "" CACHE STRING "")
set(CMAKE_CXX_FLAGS "" CACHE STRING "")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_INSTALL_PREFIX ${INSTALL_DIR} CACHE STRING "Installation directory")

project(timemory-tutorials-apps
    VERSION     0.0.1
    LANGUAGES   C CXX)

option(USE_MPI "Enable MPI support" ON)
option(USE_CUDA "Enable CUDA support" OFF)
option(USE_TIMEMORY "Enable timemory support" OFF)
option(BUILD_SHARED_LIBS "Build dynamic libraries" ON)

set(CMAKE_INSTALL_MESSAGE LAZY)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "")
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON CACHE BOOL "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING "Executable output directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "Shared library output directory")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "Static library output directory")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "CMake build type" FORCE)
endif()

set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "Enable Serial")
set(Kokkos_ENABLE_OPENMP ON CACHE BOOL "Enable OpenMP")
if(USE_CUDA)
    set(Kokkos_ENABLE_CUDA ON CACHE BOOL "Enable CUDA")
    set(Kokkos_ENABLE_CUDA_UVM ON CACHE BOOL "Enable CUDA UVM")
    set(Kokkos_ENABLE_CUDA_LAMBDA ON CACHE BOOL "Enable CUDA UVM")
    set(Kokkos_ENABLE_CUDA_CONSTEXPR ON CACHE BOOL "Enable CUDA UVM")
endif()

add_subdirectory(basic)
add_subdirectory(custom-dynamic-instr)

if(USE_CUDA)
    set(CMAKE_CXX_STANDARD 14)
endif()
add_subdirectory(kokkos)
add_subdirectory(lulesh)
# add_subdirectory(tomopy)
add_subdirectory(matvec)
