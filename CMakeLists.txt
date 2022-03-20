cmake_minimum_required (VERSION 3.20)
project(ArduinoHost)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CORELIB_NAME "${PROJECT_NAME}-lib")

message("root  CORELIB_NAME = ${CORELIB_NAME}")


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.77 REQUIRED COMPONENTS thread chrono)

add_subdirectory(api)
add_subdirectory(test)
