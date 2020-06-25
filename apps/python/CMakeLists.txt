cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(Python LANGUAGES C CXX)

find_package(PythonInterp REQUIRED)

configure_file(${PROJECT_SOURCE_DIR}/basic.py
    ${CMAKE_BINARY_DIR}/python/ex_python_basic @ONLY)

configure_file(${PROJECT_SOURCE_DIR}/intermediate.py
    ${CMAKE_BINARY_DIR}/python/ex_python_intermediate @ONLY)

configure_file(${PROJECT_SOURCE_DIR}/profiler.py
    ${CMAKE_BINARY_DIR}/python/ex_python_profiler @ONLY)

configure_file(${PROJECT_SOURCE_DIR}/standalone.py
    ${CMAKE_BINARY_DIR}/python/ex_python_standalone @ONLY)

install(
    FILES
        ${CMAKE_BINARY_DIR}/python/ex_python_basic
        ${CMAKE_BINARY_DIR}/python/ex_python_intermediate
        ${CMAKE_BINARY_DIR}/python/ex_python_profiler
        ${CMAKE_BINARY_DIR}/python/ex_python_standalone
    DESTINATION bin
    PERMISSIONS
        OWNER_EXECUTE OWNER_READ OWNER_WRITE
        GROUP_EXECUTE GROUP_READ
        WORLD_EXECUTE WORLD_READ)

if(NOT TARGET pybind11::module)
    find_package(pybind11 QUIET)
endif()

if(NOT TARGET pybind11::module)
    return()
endif()

find_package(MPI QUIET)
if(NOT MPI_FOUND)
    return()
endif()

set(_OMP)
find_package(OpenMP QUIET)
if(OpenMP_FOUND)
    set(_OMP OpenMP::OpenMP_CXX)
endif()