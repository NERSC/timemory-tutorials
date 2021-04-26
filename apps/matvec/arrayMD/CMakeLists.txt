cmake_minimum_required (VERSION 3.15)
project(matvec-arrayMD LANGUAGES C CXX)

# don't allow in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(STATUS "Warning! Building from the source directory is not allow")
  message(STATUS "Remove 'CMakeCache.txt' and 'CMakeFiles' and build from a separate directory")
  message(ERROR "In-source build")
endif()

add_library(arrayMD INTERFACE)
add_library(matvec::arrayMD ALIAS arrayMD)

target_sources(arrayMD INTERFACE ${PROJECT_SOURCE_DIR}/arrayMD.h)
target_include_directories(arrayMD INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>)
install(FILES ${PROJECT_SOURCE_DIR}/arrayMD.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
