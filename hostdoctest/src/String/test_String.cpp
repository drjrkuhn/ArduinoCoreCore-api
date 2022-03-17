#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "WString.h"

// Convert Catch TEST_CASE to doctest TEST_CASE
// 
// Regex input: TEST_CASE\("(.*)", "(.*)"\)
// Regex output: TEST_CASE("$2 $1")
// 
// test_concat.cpp              DONE
// test_operators.cpp           DONE
// test_compareTo.cpp           DONE
// test_comparisonFunc.cpp      DONE
// test_characterAccessFunc.cpp
// test_substring.cpp
// test_indexOf.cpp
// test_lastIndexOf.cpp
// test_length.cpp
// test_move.cpp
// test_remove.cpp
// test_replace.cpp
// test_String.cpp           DONE
// test_toDouble.cpp
// test_toFloat.cpp
// test_toInt.cpp
// test_toLowerCase.cpp
// test_toUpperCase.cpp
// test_trim.cpp


TEST_SUITE("[WString.h]") {


	/**************************************************************************************
	 * from test_concat.cpp
	 **************************************************************************************/
	TEST_CASE("[String-concat-01] Testing String::concat(const String &)")
	{
		arduino::String str1("Hello "), str2("Arduino!");
		REQUIRE(str1.concat(str2));
		REQUIRE(str1 == "Hello Arduino!");
	}

	TEST_CASE("[String-concat-02] Testing String::concat(const char *)")
	{
		arduino::String str("Hello ");
		REQUIRE(str.concat("Arduino!"));
		REQUIRE(str == "Hello Arduino!");
	}

	TEST_CASE("[String-concat-03] Testing String::concat(char)")
	{
		arduino::String str("Hello ");
		char const c = 'A';
		REQUIRE(str.concat(c));
		REQUIRE(str == "Hello A");
	}

	TEST_CASE("[String-concat-04] Testing String::concat(unsigned char)")
	{
		arduino::String str("Hello ");
		unsigned char const c = 'A';
		REQUIRE(str.concat(c));
		REQUIRE(str == "Hello 65"); /* ASCII['A'] = 65 */
	}

	TEST_CASE("[String-concat-05] Testing String::concat(int)")
	{
		arduino::String str("Hello ");
		int const num = -1;
		REQUIRE(str.concat(num));
		REQUIRE(str == "Hello -1");
	}

	TEST_CASE("[String-concat-06] Testing String::concat(unsigned int)")
	{
		arduino::String str("Hello ");
		unsigned int const num = 1;
		REQUIRE(str.concat(num));
		REQUIRE(str == "Hello 1");
	}

	TEST_CASE("[String-concat-07] Testing String::concat(long)")
	{
		arduino::String str("Hello ");
		long const num = -1;
		REQUIRE(str.concat(num));
		REQUIRE(str == "Hello -1");
	}

	TEST_CASE("[String-concat-08] Testing String::concat(unsigned long)")
	{
		arduino::String str("Hello ");
		unsigned long const num = 1;
		REQUIRE(str.concat(num));
		REQUIRE(str == "Hello 1");
	}

	TEST_CASE("[String-concat-09] Testing String::concat(float)")
	{
		arduino::String str("Hello ");
		float const num = 1.234f;
		REQUIRE(str.concat(num));
		REQUIRE(str == "Hello 1.23");
	}

	TEST_CASE("[String-concat-10] Testing String::concat(double)")
	{
		arduino::String str("Hello ");
		double const num = 5.678;
		REQUIRE(str.concat(num));
		REQUIRE(str == "Hello 5.68");
	}

	TEST_CASE("[String-concat-11] Testing String::concat(const __FlashStringHelper *)")
	{
#undef F
#define F(string_literal) (reinterpret_cast<const arduino::__FlashStringHelper *>(PSTR(string_literal)))
		arduino::String str1("Hello");
		REQUIRE(str1.concat(F(" Arduino")));
		REQUIRE(str1 == "Hello Arduino");
	}

	/**************************************************************************************
	 * from test_operators.cpp
	 **************************************************************************************/
#define TEST_CAST(A,B) TEST_CASE(B##A)

	TEST_CASE("[String-operator+-01] Testing String::operator + (const StringSumHelper, const String)")
	{
		arduino::String str1("Hello ");
		arduino::String str2("Arduino");
		arduino::String str("Hello Arduino");
		REQUIRE(str == str1 + str2);
	}

	TEST_CASE("[String-operator+-02] Testing String::operator + (const StringSumHelper, const char *)")
	{
		arduino::String str1("Hello ");
		arduino::String str("Hello Arduino");
		REQUIRE(str == str1 + "Arduino");
	}

	TEST_CASE("[String-operator+-03] Testing String::operator + (const StringSumHelper, char)")
	{
		arduino::String str1("Hello");
		char ch = '!';
		arduino::String str("Hello!");
		REQUIRE(str == str1 + ch);
	}

	TEST_CASE("[String-operator+-04] Testing String::operator + (const StringSumHelper, unsigned char)")
	{
		arduino::String str1("Hello ");
		unsigned char ch = 'A';
		arduino::String str("Hello 65"); /* ASCII['A'] = 65 */
		REQUIRE(str == str1 + ch);
	}

	TEST_CASE("[String-operator+-05] Testing String::operator + (const StringSumHelper, int)")
	{
		arduino::String str1("Hello ");
		arduino::String str("Hello 1");
		REQUIRE(str == str1 + 1);
	}

	TEST_CASE("[String-operator+-06] Testing String::operator + (unsigned int)")
	{
		arduino::String str1("Hello ");
		unsigned int const num = 1;
		arduino::String str("Hello 1");
		REQUIRE(str == str1 + num);
	}

	TEST_CASE("[String-operator+-07] Testing String::operator + (long)")
	{
		arduino::String str1("Hello ");
		long const num = -1;
		arduino::String str("Hello -1");
		REQUIRE(str == str1 + num);
	}

	TEST_CASE("[String-operator+-08] Testing String::operator + (unsigned long)")
	{
		arduino::String str1("Hello ");
		unsigned long const num = 1;
		arduino::String str("Hello 1");
		REQUIRE(str == str1 + num);
	}

	TEST_CASE("[String-operator+-09] Testing String::operator + (float)")
	{
		arduino::String str1("Hello ");
		float const num = 1.234f;
		arduino::String str("Hello 1.23");
		REQUIRE(str == str1 + num);
	}

	TEST_CASE("[String-operator+-10] Testing String::operator + (double)")
	{
		arduino::String str1("Hello ");
		double const num = 5.678;
		arduino::String str("Hello 5.68");
		REQUIRE(str == str1 + num);
	}

	TEST_CASE("[String-operator+-11] Testing String::operator + (const __FlashStringHelper *)")
	{
#undef F
#define F(string_literal) (reinterpret_cast<const arduino::__FlashStringHelper *>(PSTR(string_literal)))
		arduino::String str1("Hello ");
		arduino::String str("Hello Arduino");
		REQUIRE(str == str1 + F("Arduino"));
	}

	TEST_CASE("[String-operator+-12] Testing & String::operator = (StringSumHelper &&rval)")
	{
		arduino::String str1("Hello ");
		arduino::String str = (str1 + "Arduino");
		REQUIRE(str == "Hello Arduino");
	}

	TEST_CASE("[String-operator+-13] Testing & String::operator = (const String &) with invalid buffer of second string")
	{
		arduino::String str1("Hello");

		char* buffer2 = NULL;
		arduino::String str2(buffer2);

		str1 = str2;
		REQUIRE(str1 == str2);
	}

	TEST_CASE("[String-operator+-14] Testing & String::operator = (const char *) with NULL does not leave string unchanged")
	{
		char* buffer = NULL;
		arduino::String str("Hello");
		str = buffer;
		REQUIRE(str != "Hello");
	}

	TEST_CASE("[String-operator+-14] Testing & String::operator = (const char *) with NULL produces invalid string")
	{
		char* buffer = NULL;
		arduino::String str("Hello");
		str = buffer;
		REQUIRE_FALSE(str);
	}

	TEST_CASE("[String-operator+-15] Testing & String::operator = (const String &) with invalid buffer of first string")
	{
		char* buffer1 = NULL;
		arduino::String str1(buffer1);

		arduino::String str2("Hello");

		str1 = str2;
		REQUIRE(str1 == str2);
	}

	TEST_CASE("[String-operator+-16] Testing & String::operator = (String &&)")
	{
		arduino::String str("Hello");
		arduino::String str1("Arduino");
		str1 = static_cast<arduino::String&&>(str);
		REQUIRE(str1 == "Hello");
	}

	TEST_CASE("[String-operator+-17] Testing & String::operator = (StringSumHelper &&)")
	{
		arduino::String str("Hello");
		char const ch = '!';
		arduino::String str1("Arduino");
		str1 = static_cast<arduino::StringSumHelper&&>(str + ch);
		REQUIRE(str1 == "Hello!");
	}

	/**************************************************************************************
	 * from test_compareTo.cpp
	 **************************************************************************************/

	TEST_CASE("[String-compareTo-01] Testing String::compareTo(const String &)")
	{
		WHEN("Strings are equal")
		{
			arduino::String str1("Hello"), str2("Hello");
			REQUIRE(str1.compareTo(str2) == 0);
		}

		WHEN("str2 is empty")
		{
			arduino::String str1("Hello"), str2;
			REQUIRE(str1.compareTo(str2) == strcmp(str1.c_str(), str2.c_str()));
		}

		WHEN("str1 is empty")
		{
			arduino::String str1, str2("Hello");
			REQUIRE(str1.compareTo(str2) == strcmp(str1.c_str(), str2.c_str()));
		}
	}

	TEST_CASE("[String-compareTo-02] Testing String::compareTo(const char *)")
	{
		WHEN("Strings are equal")
		{
			arduino::String str("Hello");
			REQUIRE(str.compareTo("Hello") == 0);
		}

		WHEN("Passed string is empty")
		{
			arduino::String str1("Hello"), str2("");
			REQUIRE(str1.compareTo("") == strcmp(str1.c_str(), str2.c_str()));
		}

		WHEN("Passed string is compared with empty string")
		{
			arduino::String str1, str2("Hello");
			REQUIRE(str1.compareTo("Hello") == strcmp(str1.c_str(), str2.c_str()));
		}
	}

	TEST_CASE("[String-compareTo-03] Testing String::compareTo(const char *) with empty buffer")
	{
		WHEN("First string has a valid buffer")
		{
			char* buffer = NULL;

			arduino::String str1("Hello");
			REQUIRE(str1.compareTo(buffer) != 0);
		}

		WHEN("First string does NOT have a valid buffer")
		{
			char* buffer1 = NULL;
			char* buffer2 = NULL;

			arduino::String str1(buffer1);
			REQUIRE(str1.compareTo(buffer2) == 0);
		}
	}


	TEST_CASE("[String-compareTo-04] Testing String::compareTo(const String &) with empty buffer")
	{
		WHEN("First string has a valid buffer")
		{
			char* buffer = NULL;

			arduino::String str1("Hello");
			arduino::String str2(buffer);
			REQUIRE(str1.compareTo(str2) != 0);
		}

		WHEN("First string does NOT have a valid buffer")
		{
			char* buffer1 = NULL;
			char* buffer2 = NULL;

			arduino::String str1(buffer1);
			arduino::String str2(buffer2);
			REQUIRE(str1.compareTo(str2) == 0);
		}
	}

	/**************************************************************************************
	* from test_comparisonFunc.cpp
	**************************************************************************************/

	TEST_CASE("[String-equals-01] Testing String::equals(const String &) with exit status PASS")
	{
		arduino::String str1("Hello"), str2("Hello");
		REQUIRE(str1.equals(str2) == true);
	}

	TEST_CASE("[String-equals-01] Testing String::operator==(const String &) with exit status PASS")
	{
		arduino::String str1("Hello"), str2("Hello");
		REQUIRE(str1 == str2);
	}

	TEST_CASE("[String-equals-01] Testing String::operator!=(const String &) with exit status FAIL")
	{
		arduino::String str1("Hello"), str2("Hello");
		REQUIRE_FALSE(str1 != str2);
	}

	TEST_CASE("[String-equals-02] Testing String::equals(const String &) with exit status FAIL")
	{
		arduino::String str1("Hello"), str2("World");
		REQUIRE(str1.equals(str2) == false);
	}

	TEST_CASE("[String-equals-02] Testing String::operator==(const String &) with exit status FAIL")
	{
		arduino::String str1("Hello"), str2("World");
		REQUIRE_FALSE(str1 == str2);
	}

	TEST_CASE("[String-equals-02] Testing String::operator !=(const String &) with exit status PASS")
	{
		arduino::String str1("Hello"), str2("World");
		REQUIRE(str1 != str2);
	}

	TEST_CASE("[String-equals-03] Testing String::equals(const char *) with exit status PASS")
	{
		arduino::String str1("Hello");
		REQUIRE(str1.equals("Hello") == true);
	}

	TEST_CASE("[String-equals-03] Testing String::operator ==(const char *) with exit status PASS")
	{
		arduino::String str1("Hello");
		REQUIRE(str1 == "Hello");
	}

	TEST_CASE("[String-equals-03] Testing String::operator !=(const char *) with exit status FAIL")
	{
		arduino::String str1("Hello");
		REQUIRE_FALSE(str1 != "Hello");
	}

	TEST_CASE("[String-equals-04] Testing String::equals(const char *) with exit status FAIL")
	{
		arduino::String str1("Hello");
		REQUIRE(str1.equals("World") == false);
	}

	TEST_CASE("[String-equals-04] Testing String::operator ==(const char *) with exit status FAIL")
	{
		arduino::String str1("Hello");
		REQUIRE_FALSE(str1 == "World");
	}

	TEST_CASE("[String-equals-04] Testing String::operator !=(const char *) with exit status PASS")
	{
		arduino::String str1("Hello");
		REQUIRE(str1 != "World");
	}

	TEST_CASE("[String-equalsIgnoreCase-05] Testing String::equalsIgnoreCase(const String &) PASS with NON-empty string")
	{
		arduino::String str1("Hello"), str2("Hello");
		REQUIRE(str1.equalsIgnoreCase(str2) == true);
	}

	TEST_CASE("[String-equalsIgnoreCase-06] Testing String::equalsIgnoreCase(const String &) FAIL with NON-empty string")
	{
		arduino::String str1("Hello"), str2("Hel");
		REQUIRE(str1.equalsIgnoreCase(str2) == false);
	}

	TEST_CASE("[String-equalsIgnoreCase-07] Testing String::equalsIgnoreCase(const String &) FAIL with different strings")
	{
		arduino::String str1("Hello"), str2("World");
		REQUIRE(str1.equalsIgnoreCase(str2) == false);
	}

	TEST_CASE("[String-equalsIgnoreCase-08] Testing String::equalsIgnoreCase(const String &) PASS with same string")
	{
		arduino::String str1("Hello");
		REQUIRE(str1.equalsIgnoreCase(str1) == true);
	}

	TEST_CASE("[String-startsWith-09] Testing String::startsWith(const String &)")
	{
		WHEN("str2 is larger than str1")
		{
			arduino::String str1("Hello");
			arduino::String str2("Hello World");
			REQUIRE(str1.startsWith(str2) == false);
		}
		WHEN("str1 starts with str2")
		{
			arduino::String str1("Hello World");
			arduino::String str2("Hello");
			REQUIRE(str1.startsWith(str2) == true);
		}
		WHEN("str1 does NOT start with str2")
		{
			arduino::String str1("Hello World");
			arduino::String str2("Helo");
			REQUIRE(str1.startsWith(str2) == false);
		}
	}

	TEST_CASE("[String-endsWith-10] Testing String::endsWith(const String &)")
	{
		WHEN("str2 is larger than str1")
		{
			arduino::String str1("Hello");
			arduino::String str2("Hello World");
			REQUIRE(str1.endsWith(str2) == false);
		}
		WHEN("str1 ends with str2")
		{
			arduino::String str1("Hello World");
			arduino::String str2("World");
			REQUIRE(str1.endsWith(str2) == true);
		}
		WHEN("str1 does NOT end with str2")
		{
			arduino::String str1("Hello World");
			arduino::String str2("Helo");
			REQUIRE(str1.endsWith(str2) == false);
		}
	}

	/**************************************************************************************
	 * from test_characterAccessFunc
	 **************************************************************************************/

	TEST_CASE("[String-charAt-01] Testing String::charAt(unsigned int)")
	{
		arduino::String str1("Hello");
		REQUIRE(str1.charAt(2) == 'l');
	}

	TEST_CASE("[String-setCharAt-02] Testing String::setCharAt(unsigned int, char )")
	{
		arduino::String str1("Hello");
		str1.setCharAt(1, 'a');
		REQUIRE(str1 == "Hallo");
	}

	TEST_CASE("[String-getBytes-02] Testing String::getBytes(unsigned char, unsigned int, unsigned int)")
	{
		WHEN("No bufsize") {
			arduino::String str("Hello");
			unsigned char buf[5];
			str.getBytes(buf, 0, 0);
		}

		WHEN("Index >= len") {
			arduino::String str("Hello");
			unsigned char buf[5];
			str.getBytes(buf, 5, 6);
		}

		WHEN("Valid operation") {
			arduino::String str("Hello");
			unsigned char buf[5];
			str.getBytes(buf, 5, 3);
			REQUIRE(buf[0] == 'l');
			REQUIRE(buf[1] == 'o');
		}
	}

	TEST_CASE("[String-&operator[]-03] Testing & String::operator[]")
	{
		arduino::String str("Hello");
		str[0] = 'M';
		REQUIRE(str == "Mello");
	}

	TEST_CASE("[String-&operator[]-04] Testing & String::operator[] with invalid buffer")
	{
		char* buffer = NULL;
		arduino::String str(buffer);
		str[0] = 'M';
		REQUIRE(str[0] == 0);
	}

	//////////////////////////////////////////////
	/// 
	/**************************************************************************************
	 * from test_string.cpp
	 **************************************************************************************/


	TEST_CASE("[String-Ctor-01] Testing String(const char *) constructor()")
	{
		char const CSTR[] = "Hello Arduino String Class";
		arduino::String str(CSTR);
		REQUIRE(str == CSTR);
	}

	TEST_CASE("[String-Ctor-02] Testing String(const String &) constructor()")
	{
		arduino::String str1("Hello Arduino String class"),
			str2(str1);
		REQUIRE(str1 == str2);
	}

	TEST_CASE("[String-Ctor-03] Testing String(const __FlashStringHelper) constructor()")
	{
#undef F
#define F(string_literal) (reinterpret_cast<const arduino::__FlashStringHelper *>(PSTR(string_literal)))
		arduino::String str1(F("Hello"));
		REQUIRE(str1 == "Hello");
	}

	TEST_CASE("[String-Ctor-04] Testing String(char) constructor()")
	{
		char const ch = 'A';
		arduino::String str(ch);
		REQUIRE(str == "A");
	}

	TEST_CASE("[String-Ctor-05] Testing String(unsigned char, unsigned char base = 10) constructor()")
	{
		unsigned char const val = 1;
		arduino::String str(val);
		REQUIRE(str == "1");
	}

	TEST_CASE("[String-Ctor-06] Testing String(int, unsigned char base = 10) constructor()")
	{
		int const val = -1;
		arduino::String str(val);
		REQUIRE(str == "-1");
	}

	TEST_CASE("[String-Ctor-07] Testing String(unsigned int, unsigned char base = 10) constructor()")
	{
		unsigned int const val = 1;
		arduino::String str(val);
		REQUIRE(str == "1");
	}

	TEST_CASE("[String-Ctor-08] Testing String(long, unsigned char base = 10) constructor()")
	{
		long const val = -1;
		arduino::String str(val);
		REQUIRE(str == "-1");
	}

	TEST_CASE("[String-Ctor-09] Testing String(unsigned long, unsigned char base = 10) constructor()")
	{
		unsigned long const val = 1;
		arduino::String str(val);
		REQUIRE(str == "1");
	}

	TEST_CASE("[String-Ctor-10] Testing String(float, unsigned char decimalPlaces = 2) constructor()")
	{
		WHEN("String::String (some float value)")
		{
			arduino::String str(1.234f);
			REQUIRE(str == "1.23");
		}
		WHEN("String::String (FLT_MAX)")
		{
			arduino::String str(FLT_MAX);
			REQUIRE(str == "340282346638528859811704183484516925440.00");
		}
		WHEN("String::String (-FLT_MAX)")
		{
			arduino::String str(-FLT_MAX);
			REQUIRE(str == "-340282346638528859811704183484516925440.00");
		}
	}

	TEST_CASE("[String-Ctor-11] Testing String(double, unsigned char decimalPlaces = 2) constructor()")
	{
		WHEN("String::String (some double value)")
		{
			arduino::String str(5.678);
			REQUIRE(str == "5.68");
		}
		WHEN("String::String (DBL_MAX)")
		{
			arduino::String str(DBL_MAX);
			REQUIRE(str == "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00");
		}
		WHEN("String::String (-DBL_MAX)")
		{
			arduino::String str(-DBL_MAX);
			REQUIRE(str == "-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00");
		}
	}

	TEST_CASE("[String-Ctor-12] Testing String(const __FlashStringHelper) constructor() with invalid buffer")
	{
#undef F
#define F(string_literal) (reinterpret_cast<const arduino::__FlashStringHelper *>(PSTR(string_literal)))
		char* buffer = NULL;

		arduino::String str1(F(buffer));
		REQUIRE_FALSE(str1);
	}

	TEST_CASE("[String-Ctor-13] Testing String(StringSumHelper &&) constructor()")
	{
		arduino::String str("Hello");
		char const ch = '!';
		arduino::String str1(static_cast<arduino::StringSumHelper&&>(str + ch));
		REQUIRE(str1 == "Hello!");
	}

	TEST_CASE("[String-Ctor-14] Testing String(String &&) constructor()")
	{
		arduino::String str("Hello");
		arduino::String str1(static_cast<arduino::String&&>(str));
		REQUIRE(str1 == "Hello");
	}

	TEST_CASE("[String-Ctor-15] Testing String(String &&) with move(String &rhs) from smaller to larger buffer")
	{
		arduino::String str("Hello");
		arduino::String str1("Arduino");
		str1 = static_cast<arduino::String&&>(str);
		REQUIRE(str1 == "Hello");
	}

	TEST_CASE("[String-Ctor-16] Testing String(String &&) with move(String &rhs) from larger to smaller buffer")
	{
		arduino::String str("Hello");
		arduino::String str1("Arduino");
		str = static_cast<arduino::String&&>(str1);
		REQUIRE(str == "Arduino");
	}

}