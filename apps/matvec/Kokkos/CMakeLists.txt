cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(matvec-kokkos LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Release)

if(NOT TARGET Kokkos::kokkos)
    find_package(Kokkos REQUIRED)
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/matvec.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Kokkos::kokkos
    matvec::common
    matvec::tooling)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})
