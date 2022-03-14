#pragma once

#include <string>
#include <cctype>

#include "String_helpers.h"

namespace stduino {

	/** Assume no flash memory on a host computer  */
	class __FlashStringHelper;
#undef PSTR
#define PSTR(string_literal) string_literal
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

	// The string class
	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_String : public std::basic_string<charT, traits>
	{
		// use a function pointer to allow for "if (s)" without the
		// complications of an operator bool(). for more information, see:
		// http://www.artima.com/cppsource/safebool.html
		typedef void (basic_String::* StringIfHelperType)() const;
		void StringIfHelper() const {}

		static size_t const FLT_MAX_DECIMAL_PLACES = 10;
		static size_t const DBL_MAX_DECIMAL_PLACES = FLT_MAX_DECIMAL_PLACES;

		using BASE = std::basic_string<charT, traits>;

	public:
		// string constructors
		basic_String() = default;
		basic_String(const charT* c_str) : BASE(c_str) {}
		basic_String(const __FlashStringHelper* f_str) : BASE(reinterpret_cast<const charT*>(f_str)) {}
		basic_String(const charT* c_str, size_t length) : BASE(c_str, length) {}
		basic_String(const uint8_t* c_str, size_t length) : BASE(reinterpret_cast<const charT*>(c_str), length) {}
		basic_String(const BASE& other) : BASE(other) {}
		// copy constructor
		explicit basic_String(const basic_String<charT, traits>& other) : BASE(other) {}
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || _MSVC_LANG >= 201402L
		// move constructor
		explicit basic_String(basic_String&& other) : BASE(std::move(other)) { other = ""; };
#endif
		// Character constructors
		template <typename T, std::enable_if_t< is_character_v<T>, bool> = true>
		explicit basic_String(T c)
			: BASE(1, static_cast<charT>(c)) {
		}
		// Numerical constructors
		template <typename T, std::enable_if_t< is_nonchar_arithmetic_v<T>, bool> = true>
		explicit basic_String(const T number, unsigned int bop = baseOrPlaces<T>())
			: BASE(to_stdsstring<charT, T>(number, bop)) {
		}

		// destructor
		~basic_String(void) {}

		// memory management
		// return true on success, false on failure (in which case, the string
		// is left unchanged).  reserve(0), if successful, will validate an
		// invalid string (i.e., "if (s)" will be true afterwards)
		bool reserve(unsigned int size) { BASE::reserve(size); return BASE::capacity() >= size; }

		unsigned int length(void) const { return static_cast<unsigned int>(BASE::length()); }

		// copy assignment operators
		basic_String& operator= (BASE const& rhs) { this->assign(rhs); return *this; }
		basic_String& operator= (const charT* c_str) { this->assign(c_str); return *this; }
		basic_String& operator= (const __FlashStringHelper* f_str) { this->assign(reinterpret_cast<const charT*>(f_str)); return *this; }
		basic_String& operator= (const basic_String& other) { this->assign(other); return *this; }
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || _MSVC_LANG >= 201402L
		// move assignment
		basic_String& operator= (basic_String&& other) { this->assign(std::move(other)); return *this; }
#endif

		// iterators from std::basic_string
		using BASE::begin;
		using BASE::end;
		using BASE::cbegin;
		using BASE::cend;
		using BASE::rbegin;
		using BASE::rend;
		using BASE::crbegin;
		using BASE::crend;
		using BASE::c_str;

		// concatenate (works w/ built-in types)
		bool concat(const basic_String& rhs) { BASE::append(rhs); return true; }
		bool concat(const BASE& rhs) { BASE::append(rhs); return true; }
		// append pointer to string with length
		template<class strT, std::enable_if_t<is_character_ptr_v<strT>, bool> = true>
		bool concat(const strT rhs, size_t length) {
			BASE::append(reinterpret_cast<const charT*>(rhs), length);
			return true;
		}
		// append pointer to zero-terminated string
		template<typename strT, std::enable_if_t<is_character_ptr_v<strT>, bool> = true>
		bool concat(const strT rhs) {
			BASE::append(reinterpret_cast<const charT*>(rhs));
			return true;
		}
		// append character
		template <typename cT, std::enable_if_t<is_character_v<cT>, bool> = true>
		bool concat(cT c) {
			BASE::append(1, static_cast<charT>(c));
			return true;
		}
		// append number
		template <typename T, std::enable_if_t< is_nonchar_arithmetic_v<T>, bool> = true>
		bool concat(const T number, unsigned char baseOrPlaces = ::stduino::baseOrPlaces<T>()) {
			return concat(to_stdsstring<charT, T>(number, baseOrPlaces));
		}

		// operator+= is a proxy for concat
		template <typename rhsT>
		basic_String& operator+= (const rhsT rhs) {
			concat(rhs);
			return (*this);
		}

		// operator+ is a proxy for +=
		template <typename lhsT, typename rhsT>
		friend basic_String operator+ (const lhsT lhs, const rhsT rhs) {
			basic_String ret(lhs);
			return ret += rhs;
		}

		// validity operator
		operator StringIfHelperType() const {
			/** std::string should "always" be valid */
			return true ? &basic_String::StringIfHelper : 0;
		}

		// comparison (only works w/ Strings and "strings")
		int compareTo(const BASE& s) const { return BASE::compare(s); }
		int compareTo(const basic_String& s) const { return BASE::compare(s); }
		int compareTo(const char* cstr) const { return BASE::compare(cstr); }

		bool equals(const BASE& s) const { return 0 == compareTo(s); }
		bool equals(const basic_String& s) const { return 0 == compareTo(s); }
		bool equals(const char* cstr) const { return 0 == compareTo(cstr); }

		bool equalsIgnoreCase(const basic_String& b) const {
			return length() == b.length() &&
				std::equal(begin(), end(), b.begin(), b.end(),
					[](charT a, charT b) { return tolower(a) == tolower(b); }
			);
		}
		bool startsWith(const basic_String& prefix) const {
			return 0 == BASE::compare(0, prefix.length(), prefix);
		}
		bool startsWith(const basic_String& prefix, unsigned int offset) const {
			return 0 == BASE::compare(offset, prefix.length(), prefix);
		}
		bool endsWith(const basic_String& suffix) const {
			size_t slen = suffix.length();
			return 0 == BASE::compare(length() - slen, slen, suffix);
		}

		// character access
		charT charAt(unsigned int index) const { return operator[](index); }
		void setCharAt(unsigned int index, charT c) { (*this)[index] = c; }
		charT operator[] (unsigned int index) const {
			if (index < 0 || index > length()) return 0;
			return BASE::operator[](index);
		}
		charT& operator[] (unsigned int index) {
			static charT dummy;
			if (index < 0 || index > length()) return dummy;
			return BASE::operator[](index);
		}
		void getBytes(uint8_t* buf, unsigned int bufsize, unsigned int index = 0) const {
			int len = length();
			if (!bufsize || !buf) return;
			if (index >= len) {
				buf[0] = 0;
			}
			else {
				unsigned int n = bufsize - 1;
				if (n > len - index) n = len - index;
				std::copy_n(begin() + index, n, reinterpret_cast<charT*>(buf));
				buf[n] = 0;
			}
		}
		void toCharArray(charT* buf, unsigned int bufsize, unsigned int index = 0) const {
			getBytes(reinterpret_cast<uint8_t*>(buf), bufsize, index);
		}

		// search
		int indexOf(charT ch, unsigned int fromIndex = 0) const {
			return posToIndex(BASE::find(ch, fromIndex));
		}
		int indexOf(const basic_String& str, unsigned int fromIndex = 0) const {
			return posToIndex(BASE::find(str, fromIndex));
		}
		int lastIndexOf(charT ch, unsigned int fromIndex = UINT_MAX) const {
			return posToIndex(BASE::rfind(ch, indexToPos(fromIndex)));
		}
		int lastIndexOf(const basic_String& str, unsigned int fromIndex = UINT_MAX) const {
			return posToIndex(BASE::rfind(str, indexToPos(fromIndex)));
		}
		basic_String substring(unsigned int beginIndex, unsigned int endIndex = UINT_MAX) const {
			size_t len = length();
			size_t first = indexToPos(beginIndex);
			if (first > len) first = len;
			size_t last = indexToPos(endIndex);
			if (last > len) last = len;
			return BASE::substr(first, last - first);
		}

		// modification
		void replace(charT find, charT replace) {
			size_t pos = 0;
			while ((pos = BASE::find(find, pos)) != BASE::npos) {
				(*this)[pos++] = replace;
			}
		}
		void replace(const basic_String& find, const basic_String& replace) {
			size_t len = find.length();
			size_t pos = 0;
			while ((pos = BASE::find(find, pos)) != BASE::npos) {
				BASE::replace(pos, len, replace);
			}
		}
		void remove(unsigned int index, unsigned int count = 1) {
			BASE::erase(index, count);
		}
		void toLowerCase(void) {
			auto b = begin();
			auto e = end();
			auto fn = [](char c) -> char { return std::tolower(c);};
			std::transform(b, e, e, fn);
			// std::transform(begin(), end(), end(), [](char c) -> char {return std::tolower(c);});
		}
		void toUpperCase(void) {
			std::transform(begin(), end(), begin(), [](char c) -> char {return std::toupper(c);});
		}
		void trim(void) {
			typename BASE::iterator left = std::find_if_not(begin(), end(), [](char c) {return std::isspace(c);});
			if (left != begin()) {
				BASE::erase(begin(), left);
			}
			typename BASE::reverse_iterator right = std::find_if_not(rbegin(), rend(), [](char c) {return std::isspace(c);});
			if (right != rbegin()) {
				BASE::erase(right.base(), end());
			}
		}
		// parsing/conversion
		long toInt(void) const { return ::atol(c_str()); }
		float toFloat(void) const { return static_cast<float>(::atof(c_str())); }
		double toDouble(void) const { return ::atof(c_str()); }
	protected:
		static int posToIndex(size_t pos) {
			return pos == BASE::npos ? -1 : static_cast<int>(pos);
		}
		static size_t indexToPos(unsigned int index) {
			return index == INT_MAX ? BASE::npos : static_cast<size_t>(index);
		}
	};
	
	using String = basic_String<char>;
} // namespace arduino

#if defined(DOCTEST_LIBRARY_INCLUDED) && defined(DOCTEST_NEW_STRING)
#include <iostream>
#include <doctest/doctest.h>

TEST_SUITE("[new_String.h]") {
	using namespace stduino;


	TEST_CASE("constructors") {
		CHECK(String() == "");
		CHECK(String("hello") == "hello");
		CHECK(String(F("hello")) == "hello");
		CHECK(String("hello", 5) == "hello");
		CHECK(String("hello", 4) == "hell");
		CHECK(String(reinterpret_cast<const uint8_t*>("hello"), 5) == "hello");
		CHECK(String(reinterpret_cast<const uint8_t*>("hello"), 4) == "hell");
		std::string stdother = "world";
		CHECK(String(stdother) == "world");
		String other(stdother);
		// copy constructor
		CHECK(String(other) == "world");
		CHECK(other == "world");
		// move of standard string
		std::string owner1("mine");
		std::string owner2(std::move(owner1));
		CHECK(owner2=="mine");
		CHECK(owner1=="");
		// move constructor
		CHECK(String(std::move(other)) == "world");
		CHECK(other == "");
	}

	TEST_CASE_TEMPLATE("constructor chars", N, SU_CHARACTERS) {
		CHECK(String(N('c')) == "c");
	}
	TEST_CASE_TEMPLATE("constructor integrals", N, SU_INTEGRALS) {
		CHECK(String(N(100)) == "100");
	}
	TEST_CASE_TEMPLATE("constructor integrals HEX", N, SU_INTEGRALS) {
		CHECK(String(N(100), HEX) == "64");
	}
	TEST_CASE_TEMPLATE("constructor signed integrals", N, SU_SIGNED_INTEGRALS) {
		CHECK(String(N(-100)) == "-100");
	}
	TEST_CASE_TEMPLATE("constructor floats", N, SU_FLOATS) {
		CHECK(String(N(100)) == "100.00");
		CHECK(String(N(-100)) == "-100.00");
	}
	TEST_CASE_TEMPLATE("constructor floats 4 places", N, SU_FLOATS) {
		CHECK(String(N(100), 4) == "100.0000");
	}

	TEST_CASE("reserve and capacity") {
		String big("big");
		INFO("capacity = ", big.capacity());
		CHECK(big.length() == 3);
		CHECK(big.reserve(500));
		CHECK(big.capacity() >= 500);
	}
	TEST_CASE("assignment") {
		String lhs("hello"), other("other");
		std::string stdother("stdother");
		CHECK((lhs = other) == "other");
		CHECK((lhs = stdother) == "stdother");
		CHECK((lhs = "hello") == "hello");
		CHECK((lhs = F("hello")) == "hello");
		// copy assignment
		CHECK((lhs = other) == "other");
		CHECK(other == "other");
		// move assignment
		CHECK((lhs = std::move(other)) == "other");
		CHECK(other == "");
		lhs.clear();
		CHECK(lhs == "");


	}

	TEST_CASE_TEMPLATE("concat strings ptrs", PTR, SU_CHARACTER_PTRS) {
		String hello = "hello ";
		CHECK(hello.concat(PTR("world")));
		CHECK(hello == "hello world");
		hello = "hello ";
		CHECK(hello.concat(PTR("world"), 4));
		CHECK(hello == "hello worl");
	}
	TEST_CASE("concat strings classes") {
		String world("world");
		std::string stdworld("world");
		String hello = "hello ";
		CHECK(hello.concat(world));
		CHECK(hello == "hello world");
		hello = "hello ";
		CHECK(hello.concat(stdworld));
		CHECK(hello == "hello world");
		hello = "hello ";
		CHECK(hello.concat(F("world")));
		CHECK(hello == "hello world");
	}
	TEST_CASE_TEMPLATE("concat characters", C, SU_CHARACTERS) {
		String hello = "hello ";
		CHECK(hello.concat(C('w')));
		CHECK(hello == "hello w");
	}
	TEST_CASE_TEMPLATE("concat integrals", N, SU_INTEGRALS) {
		String res = "res:";
		CHECK(res.concat(N(100)));
		CHECK(res == "res:100");
	}
	TEST_CASE_TEMPLATE("concat signed integrals", N, SU_SIGNED_INTEGRALS) {
		String res = "res:";
		CHECK(res.concat(N(-100)));
		CHECK(res == "res:-100");
	}
	TEST_CASE_TEMPLATE("concat floats", N, SU_FLOATS) {
		String res = "res:";
		CHECK(res.concat(N(100.0)));
		CHECK(res == "res:100.00");
		res = "res:";
		CHECK(res.concat(N(-100.0)));
		CHECK(res == "res:-100.00");
	}
	TEST_CASE_TEMPLATE("concat integrals HEX", N, SU_INTEGRALS) {
		String res = "res:";
		CHECK(res.concat(N(100), HEX));
		CHECK(res == "res:64");
	}
	TEST_CASE_TEMPLATE("concat integrals OCT", N, SU_INTEGRALS) {
		String res = "res:";
		CHECK(res.concat(N(100), OCT));
		CHECK(res == "res:144");
	}
	TEST_CASE_TEMPLATE("concat floats 4 places", N, SU_FLOATS) {
		String res = "res:";
		CHECK(res.concat(N(100.0), 4));
		CHECK(res == "res:100.0000");
		res = "res:";
		CHECK(res.concat(N(-100.0), 4));
		CHECK(res == "res:-100.0000");
	}
	TEST_CASE_TEMPLATE("operator+= strings ptrs", PTR, SU_CHARACTER_PTRS) {
		String hello = "hello ";
		CHECK((hello += PTR("world")) == "hello world");
	}
	TEST_CASE("operator+= strings classes") {
		String hello = "hello ";
		String world("world");
		std::string stdworld("world");
		CHECK((hello += world) == "hello world");
		hello = "hello ";
		CHECK((hello += stdworld) == "hello world");
	}
	TEST_CASE_TEMPLATE("operator+= characters", C, SU_CHARACTERS) {
		String hello = "hello ";
		CHECK((hello += C('w')) == "hello w");
	}
	TEST_CASE_TEMPLATE("operator+= integrals", N, SU_INTEGRALS) {
		String res = "res:";
		CHECK((res += N(100)) == "res:100");
	}
	TEST_CASE_TEMPLATE("operator+= floats", N, SU_FLOATS) {
		String res = "res:";
		CHECK((res += N(100.0)) == "res:100.00");
	}


	TEST_CASE_TEMPLATE("operator+ strings ptrs", PTR, SU_CHARACTER_PTRS) {
		String hello = "hello ";
		String res = hello + PTR("world");
		CHECK(res == "hello world");
	}
	TEST_CASE("operator+ strings classes") {
		String hello = "hello ";
		CHECK((hello + String("world")) == "hello world");
		CHECK(hello == "hello ");
		CHECK((hello + std::string("world")) == "hello world");
		CHECK(hello == "hello ");
	}
	TEST_CASE_TEMPLATE("operator+ characters", C, SU_CHARACTERS) {
		String hello = "hello ";
		CHECK((hello + C('w')) == "hello w");
	}
	TEST_CASE_TEMPLATE("operator+ integrals", N, SU_INTEGRALS) {
		String res = "res:";
		CHECK((res + N(100)) == "res:100");
	}
	TEST_CASE_TEMPLATE("operator+ floats", N, SU_FLOATS) {
		String res = "res:";
		CHECK((res + N(100.0)) == "res:100.00");
	}

	TEST_CASE("bool (valid)") {
		String str("aaa");
		CHECK(str); // valid
	}
	TEST_CASE("compreTo") {
		std::string stdaaa("aaa");
		std::string stdbbb("bbb");
		String aaa(stdaaa);
		String bbb(stdbbb);
		CHECK(aaa.compareTo("aaa") == 0);
		CHECK(aaa.compareTo(aaa) == 0);
		CHECK(aaa.compareTo(stdaaa) == 0);
		CHECK(aaa.compareTo("bbb") < 0);
		CHECK(aaa.compareTo(bbb) < 0);
		CHECK(aaa.compareTo(stdbbb) < 0);
		CHECK(bbb.compareTo("aaa") > 0);
		CHECK(bbb.compareTo(aaa) > 0);
		CHECK(bbb.compareTo(stdaaa) > 0);
	}
	TEST_CASE("equals") {
		std::string stdaaa("aaa");
		std::string stdbbb("bbb");
		String aaa(stdaaa);
		String bbb(stdbbb);
		CHECK(aaa.equals("aaa"));
		CHECK(aaa.equals(aaa));
		CHECK(aaa.equals(stdaaa));
		CHECK(aaa.equals("bbb") == false);
		CHECK(aaa.equals(bbb) == false);
		CHECK(aaa.equals(stdbbb) == false);
	}
	TEST_CASE("operators") {
		std::string stdaaa("aaa");
		std::string stdbbb("bbb");
		String aaa(stdaaa);
		String bbb(stdbbb);
		CHECK(aaa <= "aaa");
		CHECK(aaa <= aaa);
		CHECK(aaa <= stdaaa);
		CHECK(aaa < "bbb");
		CHECK(aaa < bbb);
		CHECK(aaa < stdbbb);
		CHECK(aaa != "bbb");
		CHECK(aaa != bbb);
		CHECK(aaa != stdbbb);
	}
	TEST_CASE("equals ignore case") {
		String aaa("aaa");
		CHECK(aaa.equalsIgnoreCase("aaa"));
		CHECK(aaa.equalsIgnoreCase("AAA"));
		CHECK(aaa.equalsIgnoreCase("bbb") == false);
		CHECK(aaa.equalsIgnoreCase(String("aaa")));
		CHECK(aaa.equalsIgnoreCase(String("AAA")));
		CHECK(aaa.equalsIgnoreCase(String("bbb")) == false);
		CHECK(aaa.equalsIgnoreCase(std::string("aaa")));
		CHECK(aaa.equalsIgnoreCase(std::string("AAA")));
		CHECK(aaa.equalsIgnoreCase(std::string("bbb")) == false);
	}
	TEST_CASE("starts or ends with") {
		String hello("hello world");
		CHECK(hello.startsWith("hello"));
		CHECK(hello.startsWith("llo", 2));
		CHECK(hello.endsWith("world"));
	}
	TEST_CASE("character access") {
		String hello = "hello world";
		CHECK(hello.charAt(1) == 'e');
		CHECK(hello[1] == 'e');
		CHECK(hello[100] == 0);
		hello[1] = 'E';
		CHECK(hello.charAt(1) == 'E');
		hello = "hello world";
		hello[100] = 'f';
		CHECK(hello == "hello world");
	}
	TEST_CASE("byte array") {
		String hello = "hello world";
		String res;
		int hello_len = hello.length();
		unsigned char ucbuf[20];
		hello.getBytes(ucbuf, 20);
		CHECK(String(ucbuf, hello_len) == "hello world");
		hello.getBytes(ucbuf, 6);
		CHECK(String(ucbuf, strlen((char*)ucbuf)) == "hello");
		hello.getBytes(ucbuf, 20, 6);
		CHECK(String(ucbuf, strlen((char*)ucbuf)) == "world");

		char cbuf[20];
		hello.toCharArray(cbuf, 20);
		CHECK(String(cbuf) == "hello world");
		hello.toCharArray(cbuf, 6);
		CHECK(String(cbuf) == "hello");
		hello.toCharArray(cbuf, 20, 6);
		CHECK(String(cbuf) == "world");
	}
	TEST_CASE("c_str access") {
		String hello = "hello world";
		const char* c_str = hello.c_str();
		CHECK(strcmp(c_str, "hello world") == 0);
	}
	TEST_CASE("search") {
		String hello = "hello world";
		int res;
		CHECK((res = hello.indexOf('l')) == 2);
		CHECK((res = hello.indexOf('z')) == -1);
		CHECK((res = hello.indexOf('l', 3)) == 3);
		CHECK((res = hello.indexOf('o', 3)) == 4);
		CHECK((res = hello.lastIndexOf('r')) == 8);
		CHECK((res = hello.lastIndexOf('z')) == -1);
		CHECK((res = hello.lastIndexOf('r', -1)) == 8);
		CHECK((res = hello.lastIndexOf('r', 1000)) == 8);
		CHECK((res = hello.indexOf("llo")) == 2);
		CHECK((res = hello.indexOf("zoo")) == -1);
		CHECK((res = hello.indexOf("lo", 3)) == 3);
		CHECK((res = hello.indexOf("o w", 3)) == 4);
		CHECK((res = hello.lastIndexOf("rld")) == 8);
		CHECK((res = hello.indexOf("zoo")) == -1);
		CHECK((res = hello.lastIndexOf("rld", -1)) == 8);
		CHECK((res = hello.lastIndexOf("rld", 1000)) == 8);
	}
	TEST_CASE("substring") {
		String hello = "hello world";
		String res;
		CHECK((res = hello.substring(6)) == "world");
		CHECK((res = hello.substring(6, 8)) == "wo");
		CHECK((res = hello.substring(6, 1000)) == "world");
	}
	TEST_CASE("replace") {
		String hello = "hello world";
		String res = hello;
		res.replace('l', 'z');
		CHECK(res == "hezzo worzd");
		res = hello;
		res.replace('z', 'q');
		CHECK(res == "hello world");
		res = hello;
		res.replace("world", "galaxy");
		CHECK(res == "hello galaxy");
		String willow = "the wind in the willows";
		res = willow;
		res.replace("the", "a");
		CHECK(res == "a wind in a willows");
		res = willow;
		res.replace("the", "some");
		CHECK(res == "some wind in some willows");
		res = willow;
		res.replace("foo", "bar");
		CHECK(res == willow);
	}
	TEST_CASE("remove") {
		String hello = "hello world";
		String res = hello;
		res.remove(5);
		CHECK(res == "helloworld");
		res = hello;
		res.remove(5, 3);
		CHECK(res == "hellorld");
	}
	TEST_CASE("change case") {
		String hello = "Hello World";
		String res = hello;
		res.toLowerCase();
		CHECK(res == "hello world");
		res = hello;
		res.toUpperCase();
		CHECK(res == "HELLO WORLD");
	}
	TEST_CASE("trim") {
		String hello = " \t\r\n hello world \t\r\n ";
		hello.trim();
		CHECK(hello == "hello world");
		hello = "hello world   ";
		hello.trim();
		CHECK(hello == "hello world");
		hello = "    hello world";
		hello.trim();
		CHECK(hello == "hello world");
	}
	TEST_CASE("convert to numbers") {
		CHECK(String("100").toInt() == 100);
		CHECK(String("100.00").toFloat() == doctest::Approx(100.0));
		CHECK(String("100.00").toDouble() == doctest::Approx(100.0));
	}

}

#endif // DOCTEST_LIBRARY_INCLUDED
