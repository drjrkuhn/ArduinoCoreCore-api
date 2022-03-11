#pragma once

#include <string>
#include <type_traits>

//#define ENABLE_CHAR_TO_STRING 1

namespace stduino {

	constexpr unsigned char DEC = 10;
	constexpr unsigned char HEX = 16;
	constexpr unsigned char OCT = 8;
	constexpr unsigned char BIN = 2;

	class __FlashStringHelper;

	// alias for std::basic_string<T> used below
	template<typename charT, typename traits = std::char_traits<charT>>
	using stdstring = std::basic_string<charT>;

	// New type traits
	template <typename T, typename... Ts>
	constexpr bool is_any_v = std::disjunction<std::is_same<T, Ts>...>::value;

#define SU_CHARACTERS			char, unsigned char
#define SU_CHARACTER_PTRS		char*, unsigned char*, __FlashStringHelper*
#define SU_SIGNED_INTEGRALS		short, int, long, long long
#define SU_UNSIGNED_INTEGRALS	unsigned short, unsigned int, unsigned long,  unsigned long long
#define SU_INTEGRALS			SU_SIGNED_INTEGRALS, SU_UNSIGNED_INTEGRALS
#define SU_FLOATS				float, double, long double
#define SU_BASIC_TYPES			SU_CHARACTERS, SU_INTEGRALS, SU_FLOATS

	template <typename T>
	constexpr bool is_character_v = is_any_v<T, SU_CHARACTERS>;

	template <typename T>
	constexpr bool is_character_ptr_v = is_any_v<T, SU_CHARACTER_PTRS>;

	template <typename T>
	constexpr bool is_nonchar_integral_v = is_any_v<T, SU_INTEGRALS>;

	template <typename T>
	constexpr bool is_nonchar_signed_integral_v = is_nonchar_integral_v<T> && std::is_signed_v<T>;

	template <typename T>
	constexpr bool is_nonchar_unsigned_integral_v = is_nonchar_integral_v<T> && std::is_unsigned_v<T>;

	template <typename T>
	constexpr bool is_nonchar_arithmetic_v = is_nonchar_integral_v<T> || std::is_floating_point_v<T>;

#ifdef ENABLE_CHAR_TO_STRING
	template <typename T>
	constexpr bool is_stdstringable_v = is_character_v<T> || is_nonchar_arithmetic_v<T>;
#else
	template <typename T>
	constexpr bool is_stdstringable_v = is_nonchar_arithmetic_v<T>;
#endif

	/** Integers default to DEC as their base. */
	template<typename numT, std::enable_if_t<is_nonchar_integral_v<numT>, bool> = true>
	unsigned char baseOrPlaces() { return DEC; }

	/** Floats default to 2 places after the decimal. */
	template<typename numT, std::enable_if_t<std::is_floating_point_v<numT>, bool> = true>
	unsigned char baseOrPlaces()
	{ return 2; }

#ifdef ENABLE_CHAR_TO_STRING
	/** Floats default to 2 places after the decimal. */
	template<typename numT, std::enable_if_t<is_character_v<numT>, bool> = true>
	unsigned char baseOrPlaces() { return 1; }
#endif

	/**
	 * Append the value of a generic **unsigned** integer to a generic string with a generic base.
	 *
	 * Output should be the similar to the Arduino's Print.printNumber() function.
	 */
	template<typename charT, typename numT, typename std::enable_if_t<is_nonchar_unsigned_integral_v<numT>, bool> = true>
	std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, numT number, unsigned char base = baseOrPlaces<numT>(), bool reversed = false)
	{
		if (base < 2) base = 2;
		// create the number string in reverse order by appending digits
		size_t startlen = dest.length();
		auto start = dest.end();
		do {
			numT digit = number % base;
			number = number / base;
			dest.append(1, digit < 10 ? digit + '0' : digit + 'A' - 10);
		} while (number != 0);
		if (!reversed)
			std::reverse(dest.begin() + startlen, dest.end());

		return dest;
	}

	/**
	 * Append the value of a generic **signed** integer to a generic string with a generic base.
	 *
	 * Output should be the similar to the Arduino's Print.printNumber() function.
	 * Negative numbers are only represented for base=10. For other bases, negative
	 * numbers are represented as their two's complement.
	 */
	template<typename charT, typename numT, typename std::enable_if_t<is_nonchar_signed_integral_v<numT>, bool> = true>
	std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, numT number, unsigned char base = baseOrPlaces<numT>(), bool reversed = false)
	{
		using unsignedN = std::make_unsigned<numT>::type;
		constexpr bool KEEP_REVERSED = true;
		size_t startlen = dest.length();

		if (base == 10) {
			// treat as signed
			bool isneg = number < 0;
			if (isneg) number = -number;
			appendNumber<charT, unsignedN>(dest, unsignedN(number), base, KEEP_REVERSED);
			if (isneg)
				dest.append(1, '-');
			if (!reversed)
				std::reverse(dest.begin() + startlen, dest.end());
			return dest;
		}
		else {
			// treat as unsigned
			return appendNumber<charT, unsignedN>(dest, unsignedN(number), base, reversed);
		}
	}

	/**
	 * Append the value of a generic floating point number to a generic string.
	 *
	 * Output should be the similar to the Arduino's Print.printFloat(). It does not
	 * print exponential notation.
	 */
	template<typename charT, typename numT, typename std::enable_if_t<std::is_floating_point_v<numT>, bool> = true>
	std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, numT number, unsigned char decimalPlaces = baseOrPlaces<numT>())
	{
		if (decimalPlaces < 0)
			decimalPlaces = 2;

		if (isnan(number)) return dest.append("nan");
		if (isinf(number)) return dest.append("inf");
		if (number > 4294967040.0) return dest.append("ovf");  // constant determined empirically
		if (number < -4294967040.0) return dest.append("ovf");  // constant determined empirically
														 // Handle negative numbers
		if (number < 0.0) {
			dest.append(1, '-');
			number = -number;
		}

		// Round correctly so that print(1.999, 2) prints as "2.00"
		numT rounding = 0.5;
		for (int i = 0; i < decimalPlaces; ++i)
			rounding /= 10.0;

		number += rounding;

		// Extract the integer part of the number and print it
		unsigned long int_part = static_cast<unsigned long>(number);
		numT remainder = number - static_cast<numT>(int_part);
		appendNumber(dest, int_part);

		// Print the decimal point, but only if there are digits beyond
		if (decimalPlaces > 0) {
			dest.append(1, '.');
		}

		// Extract digits from the remainder one at a time
		while (decimalPlaces-- > 0) {
			remainder *= 10.0;
			unsigned int toPrint = static_cast<unsigned int>(remainder);
			appendNumber(dest, toPrint);
			remainder -= toPrint;
		}

		return dest;
	}

	/**
	 * Convert generic integers (signed or unsigned) or floating point to a std::string.
	 * Integers take a base as the second parameter (HEX, DEC, etc). Floating points
	 * take a decimal places as the second parameter (number of digits after the decimal).
	 *
	 * Output should be the similar to the Arduino's Print.printNumber() function.
	 */
	template<typename charT, typename numT, typename std::enable_if_t<is_nonchar_arithmetic_v<numT>, bool> = true>
	std::basic_string<charT> to_stdsstring(numT number, unsigned char baseOrPlaces = baseOrPlaces<numT>())
	{
		std::basic_string<charT> res;
		return appendNumber(res, number, baseOrPlaces);
	}

#if ENABLE_CHAR_TO_STRING
	/**
	 * Add characters (signed or unsigned) to simple 1 character strings. 
	 * 
	 * Unlike integers or floats, characters should be treated directly and should not be converted to base 10.
	 */
	template<typename charT, typename cT, typename std::enable_if_t<is_character_v<cT>, bool> = true>
	std::basic_string<charT> to_stdsstring(cT c, unsigned char dummy = baseOrPlaces<cT>())
	{
		return std::basic_string<charT>(1, c);
	}
#endif

} // namespace stduino

#if defined(DOCTEST_LIBRARY_INCLUDED) && defined(DOCTEST_STRING_HELPERS)
#include <iostream>
#include <doctest/doctest.h>

TEST_SUITE("[String_helpers.h]") {
	using namespace stduino;

#ifdef ENABLE_CHAR_TO_STRING
	TEST_CASE_TEMPLATE("[String_helpers.h] DefaultBaseOrCase characters", N, SU_CHARACTERS) {
		CHECK(baseOrPlaces<N>() == 1);
	}
#endif
	TEST_CASE_TEMPLATE("[String_helpers.h] DefaultBaseOrCase integrals", N, SU_INTEGRALS) {
		CHECK(baseOrPlaces<N>() == DEC);
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] DefaultBaseOrCase floats", N, SU_FLOATS) {
		CHECK(baseOrPlaces<N>() == 2);
	}
#ifdef ENABLE_CHAR_TO_STRING
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_CHARACTERS) {
		CHECK(to_stdsstring<char, N>(N('d')) == "d");
	}
#endif
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_INTEGRALS) {
		CHECK(to_stdsstring<char, N>(N(100)) == "100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_SIGNED_INTEGRALS) {
		CHECK(to_stdsstring<char, N>(N(-100)) == "-100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_FLOATS) {
		CHECK(to_stdsstring<char, N>(N(100.0)) == "100.00");
		CHECK(to_stdsstring<char, N>(N(-100.0)) == "-100.00");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring DEC", N, SU_INTEGRALS) {
		CHECK(to_stdsstring<char, N>(N(100), DEC) == "100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring HEX", N, SU_INTEGRALS) {
		CHECK(to_stdsstring<char, N>(N(100), HEX) == "64");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring OCT", N, SU_INTEGRALS) {
		CHECK(to_stdsstring<char, N>(N(100), OCT) == "144");
	}

	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring signed DEC", N, SU_SIGNED_INTEGRALS) {
		CHECK(to_stdsstring<char, N>(N(-100), DEC) == "-100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring float 4 places", N, SU_FLOATS) {
		CHECK(to_stdsstring<char, N>(N(100.0), 4) == "100.0000");
		CHECK(to_stdsstring<char, N>(N(-100.0), 4) == "-100.0000");
	}
}

#endif // DOCTEST_LIBRARY_INCLUDED

