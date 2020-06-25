cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(Tomopy LANGUAGES C CXX)

find_package(PythonInterp REQUIRED)

configure_file(${PROJECT_SOURCE_DIR}/phantom.py
    ${CMAKE_BINARY_DIR}/phantom.py @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/phantom.py
    DESTINATION bin
    PERMISSIONS
        OWNER_EXECUTE OWNER_READ OWNER_WRITE
        GROUP_EXECUTE GROUP_READ
        WORLD_EXECUTE WORLD_READ)

configure_file(${PROJECT_SOURCE_DIR}/misc/__init__.py
    ${CMAKE_BINARY_DIR}/misc/__init__.py @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/misc/__init__.py
    DESTINATION bin/misc)

