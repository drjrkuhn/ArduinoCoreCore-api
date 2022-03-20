##########################################################################

cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE OFF)

message("hosttest  CORELIB_NAME = ${CORELIB_NAME}")

##########################################################################
project(${CMAKE_PROJECT_NAME}-test)

find_package(catch2 2 CONFIG REQUIRED)

##########################################################################

#include_directories(../hostapi)
#include_directories(include)

##########################################################################

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################################################

set(TEST_TARGET ${PROJECT_NAME})

##########################################################################

set(TEST_SRCS
	src/Common/test_makeWord.cpp
	src/Common/test_map.cpp
	src/Common/test_max.cpp
	src/Common/test_min.cpp
	src/IPAddress/test_fromString.cpp
	src/IPAddress/test_IPAddress.cpp
	src/IPAddress/test_operator_assignment.cpp
	src/IPAddress/test_operator_comparison.cpp
	src/IPAddress/test_operator_parentheses.cpp
	src/IPAddress/test_printTo.cpp
	src/Print/PrintMocks.h
	src/Print/test_clearWriteError.cpp
	src/Print/test_getWriteError.cpp
	src/Print/test_print.cpp
	src/Print/test_println.cpp
	src/Print/test_availableForWrite.cpp
	src/Ringbuffer/test_available.cpp
	src/Ringbuffer/test_availableForStore.cpp
	src/Ringbuffer/test_clear.cpp
	src/Ringbuffer/test_isFull.cpp
	src/Ringbuffer/test_peek.cpp
	src/Ringbuffer/test_read_char.cpp
	src/Ringbuffer/test_store_char.cpp
	src/Stream/StreamMocks.h
	src/Stream/test_find.cpp
	src/Stream/test_findUntil.cpp
	src/Stream/test_getTimeout.cpp
	src/Stream/test_parseFloat.cpp
	src/Stream/test_parseInt.cpp
	src/Stream/test_readBytes.cpp
	src/Stream/test_readBytesUntil.cpp
	src/Stream/test_readString.cpp
	src/Stream/test_readStringUntil.cpp
	src/Stream/test_setTimeout.cpp
	src/String/test_concat.cpp
	src/String/test_operators.cpp
	src/String/test_compareTo.cpp
	src/String/test_comparisonFunc.cpp
	src/String/test_characterAccessFunc.cpp
	src/String/test_substring.cpp
	src/String/test_indexOf.cpp
	src/String/test_lastIndexOf.cpp
	src/String/test_length.cpp
	src/String/test_move.cpp
	src/String/test_remove.cpp
	src/String/test_replace.cpp
	src/String/test_String.cpp
	src/String/test_StringHelpers.cpp
	src/String/test_toDouble.cpp
	src/String/test_toFloat.cpp
	src/String/test_toInt.cpp
	src/String/test_toLowerCase.cpp
	src/String/test_toUpperCase.cpp
	src/String/test_trim.cpp
	src/WCharacter/test_isControl.cpp
	src/WCharacter/test_isDigit.cpp
	src/WCharacter/test_isHexadecimalDigit.cpp
	src/WCharacter/test_isLowerCase.cpp
	src/WCharacter/test_isPunct.cpp
	src/WCharacter/test_isSpace.cpp
	src/WCharacter/test_isUpperCase.cpp
	src/WCharacter/test_isWhitespace.cpp
)

##########################################################################

set(TEST_TARGET_SRCS  src/main.cpp ${TEST_SRCS})

##########################################################################

add_compile_definitions(HOST)
#add_compile_options(-Wall)

#set(CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   "--coverage")
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--coverage")

##########################################################################

add_executable( ${TEST_TARGET}  ${TEST_TARGET_SRCS})


add_dependencies(${TEST_TARGET}	${CORELIB_NAME})
target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2 ${CORELIB_NAME})

add_executable(work1 work/work1.cpp)
add_dependencies(work1 ${CORELIB_NAME})
target_link_libraries(work1 PRIVATE ${CORELIB_NAME})

##########################################################################

