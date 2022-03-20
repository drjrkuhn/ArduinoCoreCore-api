#include "Common.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>


namespace arduino {

using time_point = boost::chrono::time_point<boost::chrono::steady_clock>;
static time_point main_start_time = boost::chrono::steady_clock::now();

unsigned long millis() {
	using namespace boost;
	return static_cast<unsigned long>(chrono::duration_cast<chrono::milliseconds>(boost::chrono::steady_clock::now() - main_start_time).count());
}

unsigned long micros() {
	using namespace boost;
	return static_cast<unsigned long>(chrono::duration_cast<chrono::microseconds>(boost::chrono::steady_clock::now() - main_start_time).count());
}

void delay(unsigned long ms)
{
	using namespace boost;
	this_thread::sleep_for(chrono::milliseconds(ms));
}

void delayMicroseconds(unsigned long us)
{
	boost::this_thread::sleep_for(boost::chrono::microseconds(us));
}

void yield(void) {
	boost::this_thread::yield();
}


/* C++ prototypes */
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }

void init(void) {}
void initVariant(void) {}

void pinMode(pin_size_t pinNumber, PinMode pinMode) {}
void digitalWrite(pin_size_t pinNumber, PinStatus status) {}
PinStatus digitalRead(pin_size_t pinNumber) { return LOW; }
int analogRead(pin_size_t pinNumber) { return 0; }
void analogReference(uint8_t mode) {}
void analogWrite(pin_size_t pinNumber, int value) {}


unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout) { return 0; }
unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout) { return 0; }

void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val) {}
uint8_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder) { return 0; }

void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode) {}
void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param) {}
void detachInterrupt(pin_size_t interruptNumber) {}

} // namespace