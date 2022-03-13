#pragma once

#include <chrono>
#include <thread>

// try https://stackoverflow.com/questions/11709859/how-to-have-static-data-members-in-a-header-only-library
template <typename T>
struct time_helper_base {
	static T static_start_;
};

template<typename T>
T time_helper_base<T>::static_start_ = std::chrono::steady_clock::now();

using time_point = std::chrono::time_point<std::chrono::steady_clock>;
struct time_helper : protected time_helper_base<time_point> {
	static unsigned int millis() {
		time_point now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - static_start_).count();
	}
	static unsigned int micros() {
		time_point now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(now - static_start_).count();
	}
	static void delay(unsigned long ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
	static void delayMicroseconds(unsigned int us) {
		std::this_thread::sleep_for(std::chrono::microseconds(us));
	}
};



unsigned long millis() { return time_helper::millis(); }
unsigned long micros() { return time_helper::micros(); }
void delay(unsigned long ms) {
	return time_helper::delay(ms);
}
void delayMicroseconds(unsigned int us) {
	return time_helper::delayMicroseconds(us);
}

