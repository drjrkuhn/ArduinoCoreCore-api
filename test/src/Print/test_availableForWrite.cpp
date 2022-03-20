/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <iostream>
#include <Print.h>
#include <Print_std.h>
#include "PrintMocks.h"

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/


TEST_CASE("Print::availableForWrite() should return 0 if not overwritten by derived class", "[Print-availableForWrite-01]")
{
	Print_basic mock;
	REQUIRE(mock.availableForWrite() == 0);
}

TEST_CASE ("Print_stdstring::availableForWrite() should return number > 0", "[Print-availableForWrite-01]")
{
  arduino::Print_stdstring mock;
  REQUIRE(mock.availableForWrite() > 0);
}

TEST_CASE("Print_stdostream::availableForWrite(cout) should return number > 0", "[Print-availableForWrite-02]")
{
	arduino::Print_stdostream<std::ostream> mock(std::cout);
	REQUIRE(mock.availableForWrite() > 0);
}
