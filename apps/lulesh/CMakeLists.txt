cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(lulesh LANGUAGES C CXX)

include(GNUInstallDirs)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build type" FORCE)
endif()

option(USE_MPI "Enable MPI" ON)
add_library(lulesh-mpi INTERFACE)
if(USE_MPI)
    find_package(MPI REQUIRED)
    target_compile_definitions(lulesh-mpi INTERFACE USE_MPI=1)
    target_link_libraries(lulesh-mpi INTERFACE MPI::MPI_C MPI::MPI_CXX)
else()
    target_compile_definitions(lulesh-mpi INTERFACE USE_MPI=0)
endif()

find_package(Kokkos REQUIRED)

file(GLOB headers ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.hxx)
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)

add_executable(${PROJECT_NAME} ${sources} ${headers})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/includes)
target_link_libraries(${PROJECT_NAME} PRIVATE Kokkos::kokkos lulesh-mpi)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

enable_testing()
if(USE_MPI)
    add_test(NAME lulesh
        COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 8 $<TARGET_FILE:${PROJECT_NAME}> -i 100 -s 20 -p
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
else()
    add_test(NAME lulesh
        COMMAND $<TARGET_FILE:${PROJECT_NAME}> -i 100 -s 20 -p
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endif()
