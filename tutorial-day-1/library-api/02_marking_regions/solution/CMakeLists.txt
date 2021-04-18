cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(timemory-library-example LANGUAGES CXX)

find_package(timemory REQUIRED
    COMPONENTS headers cxx)
#

add_executable(library_example)
target_sources(library_example PRIVATE example.cpp)
target_link_libraries(library_example timemory::timemory)
