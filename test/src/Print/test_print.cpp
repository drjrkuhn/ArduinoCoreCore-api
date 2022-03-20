/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

 /**************************************************************************************
  * INCLUDE
  **************************************************************************************/

#include <catch.hpp>
#include <cstdint>

#include <Print.h>
#include <Printable.h>
#include <Print_std.h>
#include "PrintMocks.h"

  /**************************************************************************************
   * TEST CODE
   * sizes of int, long, long long vary with host architecture. Test with specific int
   * sizes in cstrint (c++) or stdint.h (c)
   **************************************************************************************/

TEST_CASE("Print::print(char)", "[Print-print-01]")
{
    arduino::Print_stdstring mock;

    mock.print('A');

    REQUIRE(mock.str() == "A");
}

TEST_CASE("Print::print(const String &)", "[Print-print-02]")
{
    arduino::Print_stdstring mock;
    arduino::String const str("Test String");

    mock.print(str);

    REQUIRE(mock.str() == "Test String");
}

TEST_CASE("Print::print(const char str[])", "[Print-print-03]")
{
    arduino::Print_stdstring mock;
    const char str[] = "Test String";

    mock.print(str);

    REQUIRE(mock.str() == "Test String");
}

TEST_CASE("Print::print(int16_t, int = DEC|HEX|OCT|BIN)", "[Print-print-04]")
{
    arduino::Print_stdstring mock;

    int16_t const val = -1;

    WHEN("DEC") { mock.print(val, DEC); REQUIRE(mock.str() == "-1"); }
    WHEN("HEX") { mock.print(val, HEX); REQUIRE(mock.str() == "FFFF"); }
    WHEN("OCT") { mock.print(val, OCT); REQUIRE(mock.str() == "177777"); }
    WHEN("BIN") { mock.print(val, BIN); REQUIRE(mock.str() == "1111111111111111"); }
}

TEST_CASE("Print::print(uint16_t, int = DEC|HEX|OCT|BIN)", "[Print-print-05]")
{
    arduino::Print_stdstring mock;

    uint16_t const val = 17;

    WHEN("DEC") { mock.print(val, DEC); REQUIRE(mock.str() == "17"); }
    WHEN("HEX") { mock.print(val, HEX); REQUIRE(mock.str() == "11"); }
    WHEN("OCT") { mock.print(val, OCT); REQUIRE(mock.str() == "21"); }
    WHEN("BIN") { mock.print(val, BIN); REQUIRE(mock.str() == "10001"); }
}

TEST_CASE("Print::print(int32_t, int = DEC|HEX|OCT|BIN)", "[Print-print-06]")
{
    arduino::Print_stdstring mock;

    int32_t const val = -1;

    WHEN("DEC") { mock.print(val, DEC); REQUIRE(mock.str() == "-1"); }
    WHEN("HEX") { mock.print(val, HEX); REQUIRE(mock.str() == "FFFFFFFF"); }
    WHEN("OCT") { mock.print(val, OCT); REQUIRE(mock.str() == "37777777777"); }
    WHEN("BIN") { mock.print(val, BIN); REQUIRE(mock.str() == "11111111111111111111111111111111"); }
}

TEST_CASE("Print::print(uint32_t, int = DEC|HEX|OCT|BIN)", "[Print-print-07]")
{
    arduino::Print_stdstring mock;

    uint32_t const val = 17;

    WHEN("DEC") { mock.print(val, DEC); REQUIRE(mock.str() == "17"); }
    WHEN("HEX") { mock.print(val, HEX); REQUIRE(mock.str() == "11"); }
    WHEN("OCT") { mock.print(val, OCT); REQUIRE(mock.str() == "21"); }
    WHEN("BIN") { mock.print(val, BIN); REQUIRE(mock.str() == "10001"); }
}

TEST_CASE("Print::print(int64_t, int = DEC|HEX|OCT|BIN)", "[Print-print-08]")
{
    arduino::Print_stdstring mock;

    int64_t const val = -1;

    WHEN("DEC") { mock.print(val, DEC); REQUIRE(mock.str() == "-1"); }
    WHEN("HEX") { mock.print(val, HEX); REQUIRE(mock.str() == "FFFFFFFFFFFFFFFF"); }
    WHEN("OCT") { mock.print(val, OCT); REQUIRE(mock.str() == "1777777777777777777777"); }
    WHEN("BIN") { mock.print(val, BIN); REQUIRE(mock.str() == "1111111111111111111111111111111111111111111111111111111111111111"); }
}

TEST_CASE("Print::print(uint64_t, int = DEC|HEX|OCT|BIN)", "[Print-print-09]")
{
    arduino::Print_stdstring mock;

    uint64_t const val = 17;

    WHEN("DEC") { mock.print(val, DEC); REQUIRE(mock.str() == "17"); }
    WHEN("HEX") { mock.print(val, HEX); REQUIRE(mock.str() == "11"); }
    WHEN("OCT") { mock.print(val, OCT); REQUIRE(mock.str() == "21"); }
    WHEN("BIN") { mock.print(val, BIN); REQUIRE(mock.str() == "10001"); }
}

TEST_CASE("Print::print(double, int = 2)", "[Print-print-10]")
{
    arduino::Print_stdstring mock;

    WHEN("val is a positive floating point value")
    {
        double const val = 3.1459;
        WHEN("digits = 0") { mock.print(val, 0); REQUIRE(mock.str() == "3"); }
        WHEN("digits = 1") { mock.print(val, 1); REQUIRE(mock.str() == "3.1"); }
        WHEN("digits = 2 (default)") { mock.print(val);    REQUIRE(mock.str() == "3.15"); }
        WHEN("digits = 3") { mock.print(val, 3); REQUIRE(mock.str() == "3.146"); }
        WHEN("digits = 4") { mock.print(val, 4); REQUIRE(mock.str() == "3.1459"); }
        WHEN("digits = 5") { mock.print(val, 5); REQUIRE(mock.str() == "3.14590"); }
    }

    WHEN("digits are negative")
    {
        double const val = 3.1459;
        WHEN("digits = -1") { mock.print(val, -1); REQUIRE(mock.str() == "3.15"); }
    }

    WHEN("val is a negative floating point value")
    {
        double const val = -3.1459;
        WHEN("digits = 2 (default)") { mock.print(val); REQUIRE(mock.str() == "-3.15"); }
    }

    WHEN("val is NAN") { mock.print(NAN);      REQUIRE(mock.str() == "nan"); }
    WHEN("val is INFINITY") { mock.print(INFINITY); REQUIRE(mock.str() == "inf"); }
}

TEST_CASE("Print::print(Printable)", "[Print-print-11]")
{
    arduino::Print_stdstring mock;

    PrintableInt printable(1);

    mock.print(printable);

    REQUIRE(mock.str() == "PrintableInt i = 1");
}

TEST_CASE("Print::print(unsigned char, int)", "[Print-print-12]")
{
    arduino::Print_stdstring mock;

    WHEN("DEC") { mock.print('A', DEC); REQUIRE(mock.str() == "65"); }
    WHEN("HEX") { mock.print('A', HEX); REQUIRE(mock.str() == "41"); }
    WHEN("OCT") { mock.print('A', OCT); REQUIRE(mock.str() == "101"); }
    WHEN("BIN") { mock.print('A', BIN); REQUIRE(mock.str() == "1000001"); }
}

TEST_CASE("Testing Print::print(const __FlashStringHelper *)", "[Print-print-13]")
{
#undef F
#define F(string_literal) (reinterpret_cast<const arduino::__FlashStringHelper *>(PSTR(string_literal)))
    arduino::Print_stdstring mock;

    mock.print(F("Hello flash string"));

    REQUIRE(mock.str() == "Hello flash string");
}

TEST_CASE("Print::print(const std::string &)", "[Print-print-14]")
{
    arduino::Print_stdstring mock;
    std::string const str("Test String");

    mock.print(str);

    REQUIRE(mock.str() == "Test String");
}

TEST_CASE("Print::print(const_iterator begin, const_iterator end)", "[Print-print-15]")
{
    arduino::Print_stdstring mock;
    std::string const str("Test String");

    mock.print_from(str.begin()+5, str.end());

    REQUIRE(mock.str() == "String");
}

TEST_CASE("operator<<(ostream, Print_stdstring)", "[Print-print-15]")
{
    arduino::Print_stdstring mock;
    mock.print("Hello ");
    mock.print(200);
    mock.print(' ');
    mock.print(10.0);

    std::stringstream os;
    os << mock;
    REQUIRE(os.str() == mock.str());
}

