
cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE OFF)

##########################################################################
message("hostapi  CORELIB_NAME = ${CORELIB_NAME}")

project(${CORELIB_NAME})


add_library(${CORELIB_NAME}
	ArduinoAPI.h
	Binary.h
	Client.h
	Common.cpp
	Common.h
	Compat.h
#	HardwareI2C.h
#	HardwareSerial.h
#	HardwareSPI.h
#	Interrupts.h
	IPAddress.cpp
	IPAddress.h
#	itoa.h
#	PluggableUSB.cpp
#	PluggableUSB.h
	Print.cpp
	Print.h
	Printable.cpp
	Printable.h
	Print_std.h
	RingBuffer.h
	Server.h
	Stream.cpp
	Stream.h
	Stream_std.h
	WString.cpp
	WString.h
	StringHelpers.h
	Udp.h
#	USBAPI.h
	
    deprecated/host/pgmspace.h
 )

#add_compile_definitions(USBCON=1 HAVE_HWSERIAL0=1 HAVE_HWSERIAL1=1 HAVE_HWSERIAL2=1 HAVE_HWSERIAL3=1)
#target_compile_definitions(ArduinoAPIhost  USBCON=1 HAVE_HWSERIAL0=1 HAVE_HWSERIAL1=1 HAVE_HWSERIAL2=1 HAVE_HWSERIAL3=1)

target_include_directories(${CORELIB_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET ${CORELIB_NAME} PROPERTY CXX_STANDARD 14)
target_link_libraries(${CORELIB_NAME} PRIVATE Boost::boost Boost::chrono Boost::thread)


