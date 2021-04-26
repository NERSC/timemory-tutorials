cmake_minimum_required (VERSION 3.15)
project(matvec-tooling LANGUAGES C CXX)

# build the stubs library
add_library(tooling-stubs)
add_library(matvec::tooling-stubs ALIAS tooling-stubs)
target_sources(tooling-stubs PRIVATE
    ${PROJECT_SOURCE_DIR}/interface.hpp
    ${PROJECT_SOURCE_DIR}/interface-stubs.cpp)
target_include_directories(tooling-stubs INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>)
install(TARGETS tooling-stubs DESTINATION ${CMAKE_INSTALL_LIBDIR})

# build the timemory library
if(USE_TIMEMORY)
    set(timemory_FIND_COMPONENTS_INTERFACE matvec::timemory)
    find_package(timemory REQUIRED COMPONENTS headers OPTIONAL_COMPONENTS cxx)

    add_library(tooling-timemory)
    add_library(matvec::tooling-timemory ALIAS tooling-timemory)
    target_sources(tooling-timemory PRIVATE
        ${PROJECT_SOURCE_DIR}/interface.hpp
        ${PROJECT_SOURCE_DIR}/interface.cpp
        ${PROJECT_SOURCE_DIR}/components.hpp
        ${PROJECT_SOURCE_DIR}/components.cpp
        ${PROJECT_SOURCE_DIR}/wrappers.cpp)
    target_include_directories(tooling-timemory INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>)
    target_sources(tooling-timemory
        PRIVATE     ${PROJECT_SOURCE_DIR}/interface.cpp
                    ${PROJECT_SOURCE_DIR}/components.cpp
                    ${PROJECT_SOURCE_DIR}/components.hpp)
    target_compile_definitions(tooling-timemory
        INTERFACE   USE_TIMEMORY)
    target_link_libraries(tooling-timemory
        PUBLIC      matvec::timemory
        PRIVATE     matvec::common)
    install(TARGETS tooling-timemory
        DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

# alias tooling to timemory or stubs
if(USE_TIMEMORY)
    add_library(matvec::tooling ALIAS tooling-timemory)
else()
    add_library(matvec::tooling ALIAS tooling-stubs)
endif()

install(FILES ${PROJECT_SOURCE_DIR}/interface.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
