cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(custom_dynamic_instr LANGUAGES CXX)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "CMake build type" FORCE)
endif()

# set the name of the interface library that will contain build components
set(timemory_FIND_COMPONENTS_INTERFACE timemory::custom-dynamic-instr)
# find timemory
find_package(timemory REQUIRED COMPONENTS cxx shared
    OPTIONAL_COMPONENTS compile-options)

# build the custom instrumentation
add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC timemory::custom-dynamic-instr)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
