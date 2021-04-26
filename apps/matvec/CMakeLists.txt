cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(matvec LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(common)
add_subdirectory(arrayMD)
add_subdirectory(tooling)
add_subdirectory(Serial)
add_subdirectory(Kokkos)
