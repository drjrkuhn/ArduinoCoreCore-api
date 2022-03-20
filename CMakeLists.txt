cmake_minimum_required (VERSION 3.20)
#project(ArduinoAPI)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(doctest CONFIG REQUIRED)
find_package(Boost 1.77 REQUIRED COMPONENTS thread chrono)
include("${doctest_DIR}/doctest.cmake")

add_subdirectory(hostapi)
add_subdirectory(hosttest)
