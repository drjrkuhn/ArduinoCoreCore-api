#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>



// Unsigned version
template<class charT, class N, typename std::enable_if<std::is_integral<N>::value&& std::is_unsigned<N>::value, bool>::type = true>
std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, N number, int base, bool reversed = false)
{
	if (base < 2) base = 2;
	// create the number string in reverse order by appending digits
	size_t startlen = dest.length();
	auto start = dest.end();
	do {
		N digit = number % base;
		number = number / base;
		dest.append(1, digit < 10 ? digit + '0' : digit + 'A' - 10);
	} while (number != 0);
	if (!reversed)
		std::reverse(dest.begin() + startlen, dest.end());

	return dest;
}

// Signed version
template<class charT, class N, typename std::enable_if<std::is_integral<N>::value&& std::is_signed<N>::value, bool>::type = true>
std::basic_string<charT>& appendNumber(std::basic_string<charT>& dest, N number, int base, bool reversed = false)
{
	using unsignedN = std::make_unsigned<N>::type;
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
	} else {
		// treat as unsigned
		return appendNumber<charT, unsignedN>(dest, unsignedN(number), base, reversed);
	}
}

// Unsigned version
template<class charT, class N, typename std::enable_if<std::is_integral<N>::value, bool>::type = true>
std::basic_string<charT> numberToStdString(N number, int base, bool reversed = false)
{
	std::string res;
	return appendNumber(res, number, base, reversed);
}


int main() {
	using namespace std;
	string greet = "Hello World!";
	reverse(greet.begin(), greet.end());
    cout << greet << endl;
	cout << 20 << " bin\t" << numberToStdString<char>(20, 2) << endl;
	cout << 20 << " oct\t" << numberToStdString<char>(20, 8) << endl;
	cout << 20 << " dec\t" << numberToStdString<char>(20, 10) << endl;
	cout << 20 << " hex\t" << numberToStdString<char>(20, 16) << endl;
	cout << endl;
	cout << -20 << " bin\t" << numberToStdString<char>(-20, 2) << endl;
	cout << -20 << " oct\t" << numberToStdString<char>(-20, 8) << endl;
	cout << -20 << " dec\t" << numberToStdString<char>(-20, 10) << endl;
	cout << -20 << " hex\t" << numberToStdString<char>(-20, 16) << endl;

	for (unsigned int n = 0; n < 10; n++) {
		string line = "\t octal \t";
		appendNumber(line, n, 8);
		cout << n << line << endl;
		//cout << n << "\t" << numberToSString<char>(n, 8) << endl;
	}
	return 0;
}

