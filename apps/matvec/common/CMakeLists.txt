cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(matvec-common LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Release)

# library with common set of routines
add_library(${PROJECT_NAME} SHARED)
add_library(matvec::common ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/common.cpp
    ${PROJECT_SOURCE_DIR}/common.hpp)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
