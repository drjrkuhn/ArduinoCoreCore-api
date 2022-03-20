/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <WString.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::equals(const String &) with exit status PASS", "[String-equals-01]")
{
  arduino::String str1("Hello"), str2("Hello");
  REQUIRE(str1.equals(str2) == true);
}

TEST_CASE ("Testing String::operator==(const String &) with exit status PASS", "[String-equals-01]")
{
  arduino::String str1("Hello"), str2("Hello");
  REQUIRE(str1 == str2);
}

TEST_CASE ("Testing String::operator!=(const String &) with exit status FAIL", "[String-equals-01]")
{
  arduino::String str1("Hello"), str2("Hello");
  REQUIRE_FALSE(str1 != str2);
}

TEST_CASE ("Testing String::equals(const String &) with exit status FAIL", "[String-equals-02]")
{
  arduino::String str1("Hello"), str2("World");
  REQUIRE(str1.equals(str2) == false);
}

TEST_CASE ("Testing String::operator==(const String &) with exit status FAIL", "[String-equals-02]")
{
  arduino::String str1("Hello"), str2("World");
  REQUIRE_FALSE(str1 == str2);
}

TEST_CASE ("Testing String::operator !=(const String &) with exit status PASS", "[String-equals-02]")
{
  arduino::String str1("Hello"), str2("World");
  REQUIRE(str1 != str2);
}

TEST_CASE ("Testing String::equals(const char *) with exit status PASS", "[String-equals-03]")
{
  arduino::String str1("Hello");
  REQUIRE(str1.equals("Hello") == true);
}

TEST_CASE ("Testing String::operator ==(const char *) with exit status PASS", "[String-equals-03]")
{
  arduino::String str1("Hello");
  REQUIRE(str1 == "Hello");
}

TEST_CASE ("Testing String::operator !=(const char *) with exit status FAIL", "[String-equals-03]")
{
  arduino::String str1("Hello");
  REQUIRE_FALSE(str1 != "Hello");
}

TEST_CASE ("Testing String::equals(const char *) with exit status FAIL", "[String-equals-04]")
{
  arduino::String str1("Hello");
  REQUIRE(str1.equals("World") == false);
}

TEST_CASE ("Testing String::operator ==(const char *) with exit status FAIL", "[String-equals-04]")
{
  arduino::String str1("Hello");
  REQUIRE_FALSE(str1 == "World");
}

TEST_CASE ("Testing String::operator !=(const char *) with exit status PASS", "[String-equals-04]")
{
  arduino::String str1("Hello");
  REQUIRE(str1 != "World");
}

TEST_CASE ("Testing String::equalsIgnoreCase(const String &) PASS with NON-empty string", "[String-equalsIgnoreCase-05]")
{
  arduino::String str1("Hello"), str2("Hello");
  REQUIRE(str1.equalsIgnoreCase(str2) == true);
}

TEST_CASE ("Testing String::equalsIgnoreCase(const String &) FAIL with NON-empty string", "[String-equalsIgnoreCase-06]")
{
  arduino::String str1("Hello"), str2("Hel");
  REQUIRE(str1.equalsIgnoreCase(str2) == false);
}

TEST_CASE ("Testing String::equalsIgnoreCase(const String &) FAIL with different strings", "[String-equalsIgnoreCase-07]")
{
  arduino::String str1("Hello"), str2("World");
  REQUIRE(str1.equalsIgnoreCase(str2) == false);
}

TEST_CASE ("Testing String::equalsIgnoreCase(const String &) PASS with same string", "[String-equalsIgnoreCase-08]")
{
  arduino::String str1("Hello");
  REQUIRE(str1.equalsIgnoreCase(str1) == true);
}

TEST_CASE ("Testing String::startsWith(const String &)", "[String-startsWith-09]")
{
  WHEN ("str2 is larger than str1")
  {
    arduino::String str1("Hello");
    arduino::String str2("Hello World");
    REQUIRE(str1.startsWith(str2) == false);
  }
  WHEN ("str1 starts with str2")
  {
    arduino::String str1("Hello World");
    arduino::String str2("Hello");
    REQUIRE(str1.startsWith(str2) == true);
  }
  WHEN ("str1 does NOT start with str2")
  {
    arduino::String str1("Hello World");
    arduino::String str2("Helo");
    REQUIRE(str1.startsWith(str2) == false);
  }
}

TEST_CASE ("Testing String::endsWith(const String &)", "[String-endsWith-10]")
{
  WHEN ("str2 is larger than str1")
  {
    arduino::String str1("Hello");
    arduino::String str2("Hello World");
    REQUIRE(str1.endsWith(str2) == false);
  }
  WHEN ("str1 ends with str2")
  {
    arduino::String str1("Hello World");
    arduino::String str2("World");
    REQUIRE(str1.endsWith(str2) == true);
  }
  WHEN ("str1 does NOT end with str2")
  {
    arduino::String str1("Hello World");
    arduino::String str2("Helo");
    REQUIRE(str1.endsWith(str2) == false);
  }
}

TEST_CASE("Testing String::equals(const std::string &) with exit status PASS", "[String-equals-11]")
{
    arduino::String str1("Hello");
    std::string str2("Hello");
    REQUIRE(str1.equals(str2) == true);
}

TEST_CASE("Testing String::operator==(const std::string &) with exit status PASS", "[String-equals-11]")
{
    arduino::String str1("Hello");
    std::string str2("Hello");
    REQUIRE(str1 == str2);
    REQUIRE(str2 == str1);
}

TEST_CASE("Testing String::operator!=(const std::string &) with exit status FAIL", "[String-equals-11]")
{
    arduino::String str1("Hello");
    std::string str2("Hello");
    REQUIRE_FALSE(str1 != str2);
    REQUIRE_FALSE(str2 != str1);
}

TEST_CASE("Testing String::equals(const std::string &) with exit status FAIL", "[String-equals-12]")
{
    arduino::String str1("Hello");
    std::string str2("World");
    REQUIRE(str1.equals(str2) == false);
}

TEST_CASE("Testing String::operator==(const std::string &) with exit status FAIL", "[String-equals-12]")
{
    arduino::String str1("Hello");
    std::string str2("World");
    REQUIRE_FALSE(str1 == str2);
    REQUIRE_FALSE(str2 == str1);
}

TEST_CASE("Testing String::operator !=(const std::string &) with exit status PASS", "[String-equals-12]")
{
    arduino::String str1("Hello");
    std::string str2("World");
    REQUIRE(str1 != str2);
    REQUIRE(str2 != str1);
}

TEST_CASE("Testing String::equalsIgnoreCase(const std::string &) PASS with NON-empty string", "[String-equalsIgnoreCase-13]")
{
    arduino::String str1("Hello"); 
    std::string str2("Hello");
    REQUIRE(str1.equalsIgnoreCase(str2) == true);
}

TEST_CASE("Testing String::equalsIgnoreCase(const std::string &) FAIL with NON-empty string", "[String-equalsIgnoreCase-14]")
{
    arduino::String str1("Hello");
    std::string str2("Hel");
    REQUIRE(str1.equalsIgnoreCase(str2) == false);
}

TEST_CASE("Testing String::equalsIgnoreCase(const std::string &) FAIL with different strings", "[String-equalsIgnoreCase-15]")
{
    arduino::String str1("Hello");
    std::string str2("World");
    REQUIRE(str1.equalsIgnoreCase(str2) == false);
}

TEST_CASE("Testing String::startsWith(const std::string &)", "[String-startsWith-16]")
{
    WHEN("str2 is larger than str1")
    {
        arduino::String str1("Hello");
        std::string str2("Hello World");
        REQUIRE(str1.startsWith(str2) == false);
    }
    WHEN("str1 starts with str2")
    {
        arduino::String str1("Hello World");
        std::string str2("Hello");
        REQUIRE(str1.startsWith(str2) == true);
    }
    WHEN("str1 does NOT start with str2")
    {
        arduino::String str1("Hello World");
        std::string str2("Helo");
        REQUIRE(str1.startsWith(str2) == false);
    }
}

TEST_CASE("Testing String::endsWith(const std::string &)", "[String-endsWith-17]")
{
    WHEN("str2 is larger than str1")
    {
        arduino::String str1("Hello");
        std::string str2("Hello World");
        REQUIRE(str1.endsWith(str2) == false);
    }
    WHEN("str1 ends with str2")
    {
        arduino::String str1("Hello World");
        std::string str2("World");
        REQUIRE(str1.endsWith(str2) == true);
    }
    WHEN("str1 does NOT end with str2")
    {
        arduino::String str1("Hello World");
        std::string str2("Helo");
        REQUIRE(str1.endsWith(str2) == false);
    }
}
