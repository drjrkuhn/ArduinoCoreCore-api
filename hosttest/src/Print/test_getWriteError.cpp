/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

 /**************************************************************************************
  * INCLUDE
  **************************************************************************************/

#include <catch.hpp>

#include <Print.h>

#include "PrintMocks.h"

  /**************************************************************************************
   * TEST CODE
   **************************************************************************************/

TEST_CASE("No write error has been set", "[Print-getWriteError-01]")
{
	Print_basic mock;
	REQUIRE(mock.getWriteError() == 0);
}

TEST_CASE("A write error has been set", "[Print-getWriteError-02]")
{
	Print_basic mock;
	mock.mock_setWriteError(5);
	REQUIRE(mock.getWriteError() == 5);
}
