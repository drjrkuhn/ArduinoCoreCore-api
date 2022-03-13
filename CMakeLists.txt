cmake_minimum_required (VERSION 3.18)

set(CMAKE_VERBOSE_MAKEFILE ON)
project(StandarduinoCore)

find_package(doctest CONFIG REQUIRED)
find_package(Boost 1.77 REQUIRED COMPONENTS thread chrono)
include("${doctest_DIR}/doctest.cmake")

add_subdirectory(devel)
#add_subdirectory(devel/streamutils/avr)
#add_subdirectory(api)

### Print all CMAKE variables
###
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
