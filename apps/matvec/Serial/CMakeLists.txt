cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(matvec-serial LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Release)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/matvec.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    matvec::common
    matvec::arrayMD
    matvec::tooling)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})

# never built with instrumentation
add_executable(matvec-noinst)
target_sources(matvec-noinst PRIVATE ${PROJECT_SOURCE_DIR}/matvec.cpp)
target_link_libraries(matvec-noinst PRIVATE
    matvec::common
    matvec::arrayMD
    matvec::tooling-stubs)
install(TARGETS matvec-noinst
    DESTINATION ${CMAKE_INSTALL_BINDIR})
