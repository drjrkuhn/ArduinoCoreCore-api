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
  **************************************************************************************/

TEST_CASE ("Print::println(char)", "[Print-println-01]")
{
  arduino::Print_stdstring mock;

  mock.println('A');

  REQUIRE(mock.str() == "A\r\n");
}

TEST_CASE ("Print::println(const String &)", "[Print-println-02]")
{
  arduino::Print_stdstring mock;
  arduino::String const str("Test String");

  mock.println(str);

  REQUIRE(mock.str() == "Test String\r\n");
}

TEST_CASE ("Print::println(const char str[])", "[Print-println-03]")
{
  arduino::Print_stdstring mock;
  const char str[] = "Test String";

  mock.println(str);

  REQUIRE(mock.str() == "Test String\r\n");
}

TEST_CASE ("Print::println(int, int = DEC (default))", "[Print-println-04]")
{
  arduino::Print_stdstring mock;
  int const val = -1;

  mock.println(val);

  REQUIRE(mock.str() == "-1\r\n");
}

TEST_CASE ("Print::println(unsigned int, int = DEC (default))", "[Print-println-05]")
{
  arduino::Print_stdstring mock;
  unsigned int const val = 17;

  mock.println(val);

  REQUIRE(mock.str() == "17\r\n");
}

TEST_CASE ("Print::println(long, int = DEC (default))", "[Print-println-06]")
{
  arduino::Print_stdstring mock;
  long const val = -1;

  mock.println(val);

  REQUIRE(mock.str() == "-1\r\n");
}

TEST_CASE ("Print::println(unsigned long, int = DEC (default))", "[Print-println-07]")
{
  arduino::Print_stdstring mock;
  unsigned long const val = 17;

  mock.println(val);

  REQUIRE(mock.str() == "17\r\n");
}

TEST_CASE ("Print::println(long long, int = DEC (default))", "[Print-println-08]")
{
  arduino::Print_stdstring mock;
  long long const val = -1;

  mock.println(val);

  REQUIRE(mock.str() == "-1\r\n");
}

TEST_CASE ("Print::println(unsigned long long, int = DEC|HEX|OCT|BIN)", "[Print-println-09]")
{
  arduino::Print_stdstring mock;
  unsigned long long const val = 17;

  mock.println(val);

  REQUIRE(mock.str() == "17\r\n");
}

TEST_CASE ("Print::println(double, int = 2)", "[Print-println-10]")
{
  arduino::Print_stdstring mock;
  double const val = 3.1459;

  mock.println(val);

  REQUIRE(mock.str() == "3.15\r\n");
}

TEST_CASE ("Print::println(Printable)", "[Print-println-11]")
{
  arduino::Print_stdstring mock;
  PrintableInt printable(1);

  mock.println(printable);

  REQUIRE(mock.str()  == "PrintableInt i = 1\r\n");
}

TEST_CASE ("Print::println(unsigned char, int base = DEC (default))", "[Print-println-12]")
{
  arduino::Print_stdstring mock;

  mock.println('A', DEC);

  REQUIRE(mock.str() == "65\r\n");
}

TEST_CASE ("Testing Print::println(const __FlashStringHelper *)", "[Print-println-13]")
{
#undef F
#define F(string_literal) (reinterpret_cast<const arduino::__FlashStringHelper *>(PSTR(string_literal)))
  arduino::Print_stdstring mock;

  mock.println(F("Hello flash string"));

  REQUIRE(mock.str()  == "Hello flash string\r\n");
}

TEST_CASE("Print::println(const std::string &)", "[Print-println-14]")
{
    arduino::Print_stdstring mock;
    std::string const str("Test String");

    mock.println(str);

    REQUIRE(mock.str() == "Test String\r\n");
}

TEST_CASE("Print::println(const_iterator begin, const_iterator end)", "[Print-println-15]")
{
    arduino::Print_stdstring mock;
    std::string const str("Test String");

    mock.println_from(str.begin() + 5, str.end());

    REQUIRE(mock.str() == "String\r\n");
}
