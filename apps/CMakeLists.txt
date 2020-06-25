cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(timemory-tutorials-apps LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_RUNTIME_OUTPUT_DIR ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIR ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIR ${CMAKE_BINARY_DIR})

add_subdirectory(basic)
add_subdirectory(python)
add_subdirectory(custom-dynamic-instr)

# add_subdirectory(lulesh)
# add_subdirectory(tomopy)
