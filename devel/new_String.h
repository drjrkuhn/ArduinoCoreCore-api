#pragma once

#include <string>

#include "String_helpers.h"

namespace stduino {

	/** Assume no flash memory on a host computer  */
	class __FlashStringHelper;
#undef PSTR
#define PSTR(string_literal) string_literal
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

	using uint8_ptr = uint8_t*;
	using const_uint8_ptr = const uint8_t*;
	using const_Fchar_ptr = const __FlashStringHelper*;


	// An inherited class for holding the result of a concatenation.  These
	// result objects are assumed to be writable by subsequent concatenations.
	//class StringSumHelper;


	// The string class
	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_String : public std::basic_string<charT, traits>
	{
#if 0
		friend class StringSumHelper;
		// use a function pointer to allow for "if (s)" without the
		// complications of an operator bool(). for more information, see:
		// http://www.artima.com/cppsource/safebool.html
		typedef void (String::* StringIfHelperType)() const
			void StringIfHelper() const {}
#endif
		static size_t const FLT_MAX_DECIMAL_PLACES = 10;
		static size_t const DBL_MAX_DECIMAL_PLACES = FLT_MAX_DECIMAL_PLACES;

	public:
		using stdstring = std::basic_string<charT,traits>;
		using const_charT = const charT;
		using charT_ptr = charT*;
		using const_charT_ptr = const charT*;

		// string constructors
		basic_String(const_charT_ptr c_str = "") : stdstring(c_str) {}
		basic_String(const_Fchar_ptr f_str) : stdstring(reinterpret_cast<const_charT_ptr>(f_str)) {}
		basic_String(const_charT_ptr c_str, size_t length) : stdstring(c_str, length) {}
		basic_String(const_uint8_ptr c_str, size_t length) : stdstring(reinterpret_cast<const_charT_ptr>(c_str), length) {}
		basic_String(const stdstring& other) : stdstring(other) {}
		// copy constructor
		explicit basic_String(const basic_String<charT, traits>& other) : stdstring(other) {}
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || _MSVC_LANG >= 201402L
		// move constructor
		explicit basic_String(basic_String&& other) : stdstring(std::move(other)) { };
#endif
		// Numerical & character constructors
		template <typename T, std::enable_if_t< is_stdstringable_v<T>, bool> = true>
		explicit basic_String(const T t) : stdstring(to_stdsstring<charT,T>(t)) {}

		// destructor
		~basic_String(void) { stdstring::~stdstring(); }

		// memory management
		// return true on success, false on failure (in which case, the string
		// is left unchanged).  reserve(0), if successful, will validate an
		// invalid string (i.e., "if (s)" will be true afterwards)
		bool reserve(unsigned int size) { stdstring::reserve(size); return stdstring::capacity() >= size; }

		unsigned int length(void) const { return static_cast<unsigned int>(stdstring::length()); }

		// copy assignment operators
		basic_String& operator= (stdstring const& rhs) { assign(rhs); return *this; }
		basic_String& operator= (const_charT_ptr c_str) { assign(c_str); return *this; }
		basic_String& operator= (const_Fchar_ptr f_str) { assign(reinterpret_cast<const_charT_ptr>(f_str)); return *this; }
		basic_String& operator= (const basic_String& other) { assign(other); return *this; }
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || _MSVC_LANG >= 201402L
		// move assignment
		basic_String& operator= (basic_String&& other) { assign(std::move(other)); return *this;		}
#endif

		// concatenate (works w/ built-in types)
		bool concat(const basic_String& rhs) { append(rhs); return true; }
		bool concat(const stdstring& rhs) { append(rhs); return true; }
		// append pointer to string with length
		template<class strT, std::enable_if_t<is_character_ptr_v<strT>, bool> = true>
		bool concat(const strT rhs, size_t length) { append(reinterpret_cast<const_charT_ptr>(rhs), length); return true; }
		// append pointer to zero-terminated string
		template<class strT, std::enable_if_t<is_character_ptr_v<strT>, bool> = true>
		bool concat(const strT rhs) { append(reinterpret_cast<const_charT_ptr>(rhs)); return true; }
		// Numerical & character constructors
		template <typename T, std::enable_if_t< is_stdstringable_v<T>, bool> = true>
		bool concat(const T t, unsigned char baseOrPlaces=DefaultBaseOrPlaces<T>()) { return concat(to_stdsstring<charT, T>(t, baseOrPlaces));	}

		// The += operator is a proxy for concat
		template <typename rhsT>
		basic_String& operator+= (const rhsT rhs) { concat(rhs); return (*this); }
#if 0

		//basic_String& operator+= (const String& rhs) { concat(rhs); return (*this); }
		//basic_String& operator+= (const char* cstr) { concat(cstr); return (*this); }
		//basic_String& operator+= (char c) { concat(c); return (*this); }
		//basic_String& operator+= (unsigned char num) { concat(num); return (*this); }
		//basic_String& operator+= (int num) { concat(num); return (*this); }
		//basic_String& operator+= (unsigned int num) { concat(num); return (*this); }
		//basic_String& operator+= (long num) { concat(num); return (*this); }
		//basic_String& operator+= (unsigned long num) { concat(num); return (*this); }
		//basic_String& operator+= (float num) { concat(num); return (*this); }
		//basic_String& operator+= (double num) { concat(num); return (*this); }
		//basic_String& operator+= (const __FlashStringHelper* str) { concat(str); return (*this); }

		//friend StringSumHelper& operator + (const StringSumHelper& lhs, const String& rhs);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, const char* cstr);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, char c);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, unsigned char num);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, int num);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, unsigned int num);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, long num);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, unsigned long num);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, float num);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, double num);
		//friend StringSumHelper& operator + (const StringSumHelper& lhs, const __FlashStringHelper* rhs);
		template<typename T>
		friend basic_String& operator+(const basic_String& lhs, const T rhs) { basic_String res = lhs;  res.concat(rhs); return true; }


		// comparison (only works w/ Strings and "strings")
		operator StringIfHelperType() const { return true ? &String::StringIfHelper : 0; /* std::string is always valid */ }
		int compareTo(const String& s) const;
		int compareTo(const char* cstr) const;
		bool equals(const String& s) const;
		bool equals(const char* cstr) const;

		friend bool operator == (const String& a, const String& b) { return a.equals(b); }
		friend bool operator == (const String& a, const char* b) { return a.equals(b); }
		friend bool operator == (const char* a, const String& b) { return b == a; }
		friend bool operator <  (const String& a, const String& b) { return a.compareTo(b) < 0; }
		friend bool operator <  (const String& a, const char* b) { return a.compareTo(b) < 0; }
		friend bool operator <  (const char* a, const String& b) { return b.compareTo(a) > 0; }

		friend bool operator != (const String& a, const String& b) { return !(a == b); }
		friend bool operator != (const String& a, const char* b) { return !(a == b); }
		friend bool operator != (const char* a, const String& b) { return !(a == b); }
		friend bool operator >  (const String& a, const String& b) { return b < a; }
		friend bool operator >  (const String& a, const char* b) { return b < a; }
		friend bool operator >  (const char* a, const String& b) { return b < a; }
		friend bool operator <= (const String& a, const String& b) { return !(b < a); }
		friend bool operator <= (const String& a, const char* b) { return !(b < a); }
		friend bool operator <= (const char* a, const String& b) { return !(b < a); }
		friend bool operator >= (const String& a, const String& b) { return !(a < b); }
		friend bool operator >= (const String& a, const char* b) { return !(a < b); }
		friend bool operator >= (const char* a, const String& b) { return !(a < b); }

		bool equalsIgnoreCase(const String& s) const;
		bool startsWith(const String& prefix) const;
		bool startsWith(const String& prefix, unsigned int offset) const;
		bool endsWith(const String& suffix) const;

		// character access
		char charAt(unsigned int index) const;
		void setCharAt(unsigned int index, char c);
		char operator [] (unsigned int index) const;
		char& operator [] (unsigned int index);
		void getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index = 0) const;
		void toCharArray(char* buf, unsigned int bufsize, unsigned int index = 0) const
		{
			getBytes((unsigned char*)buf, bufsize, index);
		}
		const char* c_str() const { return buffer; }
		char* begin() { return buffer; }
		char* end() { return buffer + length(); }
		const char* begin() const { return c_str(); }
		const char* end() const { return c_str() + length(); }

		// search
		int indexOf(char ch) const;
		int indexOf(char ch, unsigned int fromIndex) const;
		int indexOf(const String& str) const;
		int indexOf(const String& str, unsigned int fromIndex) const;
		int lastIndexOf(char ch) const;
		int lastIndexOf(char ch, unsigned int fromIndex) const;
		int lastIndexOf(const String& str) const;
		int lastIndexOf(const String& str, unsigned int fromIndex) const;
		String substring(unsigned int beginIndex) const { return substring(beginIndex, len); };
		String substring(unsigned int beginIndex, unsigned int endIndex) const;

		// modification
		void replace(char find, char replace);
		void replace(const String& find, const String& replace);
		void remove(unsigned int index);
		void remove(unsigned int index, unsigned int count);
		void toLowerCase(void);
		void toUpperCase(void);
		void trim(void);

		// parsing/conversion
		long toInt(void) const;
		float toFloat(void) const;
		double toDouble(void) const;

	protected:
		char* buffer;	        // the actual char array
		unsigned int capacity;  // the array length minus one (for the '\0')
		unsigned int len;       // the String length (not counting the '\0')
	protected:
		void init(void);
		void invalidate(void);
		bool changeBuffer(unsigned int maxStrLen);

		// copy and move
		String& copy(const char* cstr, unsigned int length);
		String& copy(const __FlashStringHelper* pstr, unsigned int length);
//#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
//		void move(String& rhs);
//#endif
#endif
	};

	#if 0
	class StringSumHelper : public String
	{
	public:
		StringSumHelper(const String& s) : String(s) {}
		StringSumHelper(const char* p) : String(p) {}
		StringSumHelper(char c) : String(c) {}
		StringSumHelper(unsigned char num) : String(num) {}
		StringSumHelper(int num) : String(num) {}
		StringSumHelper(unsigned int num) : String(num) {}
		StringSumHelper(long num) : String(num) {}
		StringSumHelper(unsigned long num) : String(num) {}
		StringSumHelper(float num) : String(num) {}
		StringSumHelper(double num) : String(num) {}
	};
#endif

} // namespace arduino

//#ifdef DOCTEST_LIBRARY_INCLUDED
#include <iostream>
#include <doctest/doctest.h>

TEST_SUITE("[new_String.h]") {
	using namespace stduino;
	using String = basic_String<char>;


	TEST_CASE("constructors") {
		CHECK(String() == "");
		CHECK(String("hello") == "hello");
		CHECK(String(F("hello")) == "hello");
		CHECK(String("hello", 5) == "hello");
		CHECK(String("hello", 4) == "hell");
		CHECK(String(reinterpret_cast<const_uint8_ptr>("hello"), 5) == "hello");
		CHECK(String(reinterpret_cast<const_uint8_ptr>("hello"), 4) == "hell");
		std::string stdother = "world";
		CHECK(String(stdother) == "world");
		String other(stdother);
		// copy constructor
		CHECK(String(other) == "world");
		CHECK(other == "world");
		// move constructor
		CHECK(String(std::move(other)) == "world");
		CHECK(other == "");
	}

#if 0
	TEST_CASE_TEMPLATE("constructor chars", N, SU_CHARACTERS) {
		CHECK(String(N('c')) == "c");
	}
	TEST_CASE_TEMPLATE("constructor integrals", N, SU_INTEGRALS) {
		CHECK(String(N(100)) == "100");
	}
	TEST_CASE_TEMPLATE("constructor signed integrals", N, SU_SIGNED_INTEGRALS) {
		CHECK(String(N(-100)) == "-100");
	}
	TEST_CASE_TEMPLATE("constructor integrals", N, SU_FLOATS) {
		CHECK(String(N(100)) == "100.00");
		CHECK(String(N(-100)) == "-100.00");
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
	}

	TEST_CASE_TEMPLATE("concat strings ptrs", PTR, SU_CHARACTER_PTRS) {
		String hello = "hello ";
		CHECK(hello.concat(PTR("world")));
		CHECK(hello == "hello world");
		hello = "hello ";
		CHECK(hello.concat(PTR("world"),4));
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
		CHECK(res.concat(N(100.0),4));
		CHECK(res == "res:100.0000");
		res = "res:";
		CHECK(res.concat(N(-100.0),4));
		CHECK(res == "res:-100.0000");
	}
#endif
	TEST_CASE_TEMPLATE("operator+= strings ptrs", PTR, SU_CHARACTER_PTRS) {
		String hello = "hello ";
		CHECK((hello += PTR("world")) == "hello world");
		hello = "hello ";
	}
	TEST_CASE("operator+= strings classes") {
		String world("world");
		std::string stdworld("world");
		String hello = "hello ";
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
		CHECK((res +=N(100)) == "res:100");
	}
	TEST_CASE_TEMPLATE("operator+= floats", N, SU_FLOATS) {
		String res = "res:";
		CHECK((res += N(100.0)) == "res:100.00");
	}
}

//#endif // DOCTEST_LIBRARY_INCLUDED
