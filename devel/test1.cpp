#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include "String_helpers.h"

//
//// Unsigned version
//template<class charT, class numT, typename std::enable_if<std::is_integral<numT>::value&& std::is_unsigned<numT>::value, bool>::type = true>
//std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, numT number, int base=10, bool reversed = false)
//{
//	if (base < 2) base = 2;
//	// create the number string in reverse order by appending digits
//	size_t startlen = dest.length();
//	auto start = dest.end();
//	do {
//		numT digit = number % base;
//		number = number / base;
//		dest.append(1, digit < 10 ? digit + '0' : digit + 'A' - 10);
//	} while (number != 0);
//	if (!reversed)
//		std::reverse(dest.begin() + startlen, dest.end());
//
//	return dest;
//}
//
//// Signed version
//template<class charT, class numT, typename std::enable_if<std::is_integral<numT>::value&& std::is_signed<numT>::value, bool>::type = true>
//std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, numT number, int base=10, bool reversed = false)
//{
//	using unsignedN = std::make_unsigned<numT>::type;
//	constexpr bool KEEP_REVERSED = true;
//	size_t startlen = dest.length();
//
//	if (base == 10) {
//		// treat as signed
//		bool isneg = number < 0;
//		if (isneg) number = -number;
//		appendNumber<charT, unsignedN>(dest, unsignedN(number), base, KEEP_REVERSED);
//		if (isneg) 
//			dest.append(1, '-');
//		if (!reversed)
//			std::reverse(dest.begin() + startlen, dest.end());
//		return dest;
//	} else {
//		// treat as unsigned
//		return appendNumber<charT, unsignedN>(dest, unsignedN(number), base, reversed);
//	}
//}
//
//// Unsigned version
//template<class charT, class numT, typename std::enable_if<std::is_integral<numT>::value, bool>::type = true>
//std::basic_string<charT> numberToStdString(numT number, int base = 10, bool reversed = false)
//{
//	std::string res;
//	return appendNumber(res, number, base, reversed);
//}
//
//template<class charT, class floatT, typename std::enable_if<std::is_floating_point<floatT>::value, bool>::type = true>
//std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, floatT number, int digits)
//{
//	if (digits < 0)
//		digits = 2;
//
//	if (isnan(number)) return dest.append("nan");
//	if (isinf(number)) return dest.append("inf");
//	if (number > 4294967040.0) return dest.append("ovf");  // constant determined empirically
//	if (number < -4294967040.0) return dest.append("ovf");  // constant determined empirically
//													 // Handle negative numbers
//	if (number < 0.0) {
//		dest.append(1, '-');
//		number = -number;
//	}
//
//	// Round correctly so that print(1.999, 2) prints as "2.00"
//	floatT rounding = 0.5;
//	for (int i = 0; i < digits; ++i)
//		rounding /= 10.0;
//
//	number += rounding;
//
//	// Extract the integer part of the number and print it
//	unsigned long int_part = static_cast<unsigned long>(number);
//	floatT remainder = number - static_cast<floatT>(int_part);
//	appendNumber(dest, int_part);
//
//	// Print the decimal point, but only if there are digits beyond
//	if (digits > 0) {
//		dest.append(1, '.');
//	}
//
//	// Extract digits from the remainder one at a time
//	while (digits-- > 0) {
//		remainder *= 10.0;
//		unsigned int toPrint = static_cast<unsigned int>(remainder);
//		appendNumber(dest, toPrint);
//		remainder -= toPrint;
//	}
//
//	return dest;
//}
//
//
//template<class charT, class floatT, typename std::enable_if<std::is_floating_point<floatT>::value, bool>::type = true>
//std::basic_string<charT> numberToStdString(floatT number, int digits)
//{
//	std::string res;
//	return appendNumber(res, number, digits);
//}


int main(int argc, char* argv[]) 
{
	using namespace std;
	using namespace stduino;

	uint8_t byteval = '!';
	cout << byteval << endl;


	string greet = "Hello World!";
	reverse(greet.begin(), greet.end());
    cout << greet << endl;
	cout << 20 << " bin\t" << to_stdsstring<char>(20, 2) << endl;
	cout << 20 << " oct\t" << to_stdsstring<char>(20, 8) << endl;
	cout << 20 << " dec\t" << to_stdsstring<char>(20, 10) << endl;
	cout << 20 << " hex\t" << to_stdsstring<char>(20, 16) << endl;
	cout << endl;
	cout << -20 << " bin\t" << to_stdsstring<char>(-20, 2) << endl;
	cout << -20 << " oct\t" << to_stdsstring<char>(-20, 8) << endl;
	cout << -20 << " dec\t" << to_stdsstring<char>(-20, 10) << endl;
	cout << -20 << " hex\t" << to_stdsstring<char>(-20, 16) << endl;

	for (unsigned int n = 0; n < 10; n++) {
		string line = "\t octal \t";
		appendNumber(line, n, 8);
		cout << n << line << endl;
		//cout << n << "\t" << numberToSString<char>(n, 8) << endl;
	}

	int space = 15;
	for (double mult = 0.0001; mult <= 1e10; mult *= 10) {
		double dval = M_PI;
		dval *= mult;
		cout << "double\t" << std::string(space, '.') << to_stdsstring<char, double>(dval, 15) << endl;
		space--;
	}

	std::stringstream ss;
	ss << "Stringstream ";
	appendNumber(ss.str(), 1234567890.0, 2);
	cout << ss.str() << endl;

	return 0;
}

