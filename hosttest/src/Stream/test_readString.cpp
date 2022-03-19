/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include "StreamMocks.h"

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing 'readString' with data available within the stream", "[Stream-readString-01]")
{
  StreamMock mock;
  mock.setTimeout(10);
  mock << "This is test stream content";

  REQUIRE(mock.readString() == arduino::String("This is test stream content"));
}

TEST_CASE("Testing 'readStdString' with data available within the stream", "[Stream-readString-02]")
{
	StreamMock mock;
	mock.setTimeout(10);
	mock << "This is test stream content";

	REQUIRE(mock.readStdString() == "This is test stream content");
}
