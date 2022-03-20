/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <float.h>
#include <catch.hpp>

#include "WString.h"
#include "StringHelpers.h"

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

//TEST_CASE ("Testing String(const char *) constructor()", "[String-Ctor-01]")
//{
//  char const CSTR[] = "Hello Arduino String Class";
//  arduino::String str(CSTR);
//  REQUIRE(str == CSTR);
//}
#define SU_CHARACTERS			char, unsigned char
#define SU_SIGNED_INTEGRALS		short, int, long, long long
#define SU_UNSIGNED_INTEGRALS	unsigned short, unsigned int, unsigned long, unsigned long long
#define SU_INTEGRALS			SU_SIGNED_INTEGRALS, SU_UNSIGNED_INTEGRALS
#define SU_FLOATS				float, double, long double

#define DEC	10
#define HEX 16
#define OCT 8


TEMPLATE_TEST_CASE("Testing to_string with characters", "[String-helpers-01]", SU_CHARACTERS) {
	REQUIRE(arduino::to_string<TestType>(TestType(100), DEC) == "100");
}

TEMPLATE_TEST_CASE("Testing to_string with integrals", "[String-helpers-02]", SU_INTEGRALS) {
	REQUIRE(arduino::to_string<TestType>(TestType(100), DEC) == "100");
}

TEMPLATE_TEST_CASE("Testing to_string with signed integrals", "[String-helpers-03]", SU_SIGNED_INTEGRALS) {
	REQUIRE(arduino::to_string<TestType>(TestType(-100), DEC) == "-100");
}

TEMPLATE_TEST_CASE("Testing to_string HEX with integrals", "[String-helpers-04]", SU_INTEGRALS) {
	REQUIRE(arduino::to_string<TestType>(TestType(100), HEX) == "64");
}

TEMPLATE_TEST_CASE("Testing to_string OCT with integrals", "[String-helpers-05]", SU_INTEGRALS) {
	REQUIRE(arduino::to_string<TestType>(TestType(100), OCT) == "144");
}

TEMPLATE_TEST_CASE("Testing to_string with floats 2 places", "[String-helpers-06]", SU_FLOATS) {
	REQUIRE(arduino::to_string<TestType>(TestType(100.0), 2) == "100.00");
	REQUIRE(arduino::to_string<TestType>(TestType(-100.0), 2) == "-100.00");
}

TEMPLATE_TEST_CASE("Testing to_string with floats 4 places", "[String-helpers-07]", SU_FLOATS) {
	REQUIRE(arduino::to_string<TestType>(TestType(100.0), 4) == "100.0000");
	REQUIRE(arduino::to_string<TestType>(TestType(-100.0), 4) == "-100.0000");
}
