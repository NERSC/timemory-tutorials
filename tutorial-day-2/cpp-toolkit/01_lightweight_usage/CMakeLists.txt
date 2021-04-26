cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(example-01 LANGUAGES CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(timemory_FIND_COMPONENTS_INTERFACE timemory::${PROJECT_NAME})

find_package(timemory REQUIRED
    COMPONENTS headers
    OPTIONAL_COMPONENTS cxx arch compile-options)

add_library(mm mm.hpp mm.cpp)

target_link_libraries(mm PUBLIC timemory::${PROJECT_NAME})

add_executable(${PROJECT_NAME} example.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE mm)

include(GNUInstallDirs)

install(TARGETS mm
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})
