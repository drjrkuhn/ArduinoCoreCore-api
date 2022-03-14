#pragma once

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

namespace stduino
{
	/*
	 * NOTE std::this_thread::delay is VERY inaccurate in Windows.
	 * See:
	 * https://developercommunity.visualstudio.com/t/bogus-stdthis-threadsleep-for-implementation/58530
	 *
	 * Should probably switch to boost::threads
	 */
	template <typename CLOCK, typename TP = boost::chrono::time_point<CLOCK>>
	struct time_helper
	{
		using time_point = TP;
		static time_point start;
		static time_point now() { return CLOCK::now(); }
	};

	template <typename CLOCK, typename TP>
	TP time_helper<CLOCK, TP>::start = time_helper<CLOCK, TP>::now();

	struct systime : public time_helper<boost::chrono::steady_clock>
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

	unsigned int millis() { return systime::millis(); }
	unsigned int micros() { return systime::micros(); }
	void delay(unsigned long ms) { systime::delay(ms); }
	void delayMicroseconds(unsigned long us) { systime::delayMicroseconds(us); }

}; // namespace