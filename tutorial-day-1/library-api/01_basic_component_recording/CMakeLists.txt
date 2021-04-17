cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(timemory-library-example LANGUAGES CXX)

find_package(timemory REQUIRED COMPONENTS headers compile-options OPTIONAL_COMPONENTS cxx)
#

function(GET_LIBRARY _VAR _LANG)
	string(TOLOWER ${_LANG} _LOW)
	set(_PREFIX timemory::timemory-${_LOW})
	if(TARGET ${_PREFIX}-shared)
		set(${_VAR} ${_PREFIX}-shared PARENT_SCOPE)
	elseif(TARGET ${_PREFIX}-static)
		set(${_VAR} ${_PREFIX}-static PARENT_SCOPE)
	endif()
endfunction()

get_library(TIMEMORY_CXX_LIBRARY CXX)

add_executable(library_example)
target_sources(library_example PRIVATE example.cpp)
target_link_libraries(library_example ${TIMEMORY_CXX_LIBRARY})
