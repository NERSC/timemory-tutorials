cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(Python LANGUAGES C CXX)

find_package(PythonInterp REQUIRED)

foreach(_FILE basic intermediate profiler standalone)
    configure_file(${PROJECT_SOURCE_DIR}/${_FILE}.py
        ${CMAKE_BINARY_DIR}/${_FILE}.py @ONLY)

    install(FILES ${CMAKE_BINARY_DIR}/${_FILE}.py
        DESTINATION bin
        PERMISSIONS
            OWNER_EXECUTE OWNER_READ OWNER_WRITE
            GROUP_EXECUTE GROUP_READ
            WORLD_EXECUTE WORLD_READ)
endforeach()
