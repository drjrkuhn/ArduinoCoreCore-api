/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <Print.h>
#include <Print_std.h>
#include "PrintMocks.h"

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Clear write error should set the error code back to 0", "[Print-clearWriteError-01]")
{
  Print_basic  mock;
  mock.mock_setWriteError(5);
  mock.clearWriteError();
  REQUIRE(mock.getWriteError() == 0);
}
