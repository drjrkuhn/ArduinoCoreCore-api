#include "Common.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

template <typename CLOCK, typename TP = boost::chrono::time_point<CLOCK>>
struct time_helper
{
	using time_point = TP;
	static time_point start;
	static time_point now() { return CLOCK::now(); }
};

template <typename CLOCK, typename TP>
TP time_helper<CLOCK, TP>::start = time_helper<CLOCK, TP>::now();

template <typename CLOCK>
struct basic_systime : public time_helper<CLOCK>
{
	static unsigned int millis()
	{
		return boost::chrono::duration_cast<boost::chrono::milliseconds>(now() - start).count();
	}
	static unsigned int micros()
	{
		return boost::chrono::duration_cast<boost::chrono::microseconds>(now() - start).count();
	}
	static void delay(unsigned long ms)
	{
		// timeval tv;
		// tv.tv_sec = ms / 1000;
		// tv.tv_usec = ms % 1000;
		// select(0, NULL, NULL, NULL, &tv);
		boost::this_thread::sleep_for(boost::chrono::milliseconds(ms));
	}
	static void delayMicroseconds(unsigned int us)
	{
		boost::this_thread::sleep_for(boost::chrono::microseconds(us));
	}
};

using systime = basic_systime<boost::chrono::steady_clock>;

/* C++ prototypes */
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }

void yield(void) {}

void init(void) {}

void initVariant(void) {}

void pinMode(pin_size_t pinNumber, PinMode pinMode) {}

void digitalWrite(pin_size_t pinNumber, PinStatus status) {}
PinStatus digitalRead(pin_size_t pinNumber) { return LOW; }
int analogRead(pin_size_t pinNumber) { return 0; }
void analogReference(uint8_t mode) {}
void analogWrite(pin_size_t pinNumber, int value) {}


unsigned long millis(void)
{
	return systime::millis();
}
unsigned long micros(void)
{
	return systime::micros();
}
void delay(unsigned long ms)
{
	systime::delay(ms);
}
void delayMicroseconds(unsigned int us)
{
	systime::delayMicroseconds(us);
}
unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout)
{
	return 0;
}
unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout)
{
	return 0;
}

void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val)
{
}
uint8_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder)
{
	return 0;
}

void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode)
{
}

void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param)
{
}

void detachInterrupt(pin_size_t interruptNumber)
{
}
