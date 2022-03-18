add_library(ArduinoAPIhost
#	ArduinoAPI.h
#	Binary.h
#	Client.h
	Common.cpp
	Common.h
#	Compat.h
#	HardwareI2C.h
#	HardwareSerial.h
#	HardwareSPI.h
#	Interrupts.h
#	IPAddress.cpp
#	IPAddress.h
#	itoa.h
#	PluggableUSB.cpp
#	PluggableUSB.h
	Print.cpp
	Print.h
	Printable.h
	Print_ostream.h
#	RingBuffer.h
#	Server.h
	Stream.cpp
	Stream.h
	WString.cpp
	WString.h
	StringHelpers.h
#	Udp.h
#	USBAPI.h
#	WCharacter.h
#	deprecated-avr-comp/avr/dtostrf.h
#	deprecated-avr-comp/avr/interrupt.h
#	deprecated-avr-comp/avr/pgmspace.h
    deprecated/host/pgmspace.h
)

#add_compile_definitions(USBCON=1 HAVE_HWSERIAL0=1 HAVE_HWSERIAL1=1 HAVE_HWSERIAL2=1 HAVE_HWSERIAL3=1)
#target_compile_definitions(ArduinoAPIhost  USBCON=1 HAVE_HWSERIAL0=1 HAVE_HWSERIAL1=1 HAVE_HWSERIAL2=1 HAVE_HWSERIAL3=1)

target_include_directories(ArduinoAPIhost
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET ArduinoAPIhost PROPERTY CXX_STANDARD 14)
target_link_libraries(ArduinoAPIhost PRIVATE Boost::boost Boost::chrono Boost::thread)


