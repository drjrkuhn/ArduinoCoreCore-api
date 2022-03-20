/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include "StreamMocks.h"
#include <iostream>
/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing 'readStringUntil' with separator available within the stream", "[Stream-readStringUntil-01]")
{
  StreamMock mock;
  mock.setTimeout(10);
  mock << "This is test! lorem ipsum lalala";

  REQUIRE(mock.readStringUntil('!') == arduino::String("This is test"));
}

TEST_CASE ("Testing 'readStringUntil' with separator not available within the stream", "[Stream-readStringUntil-02]")
{
  StreamMock mock;
  mock.setTimeout(10);
  mock << "This is test ... lorem ipsum lalala";

  REQUIRE(mock.readStringUntil('!') == arduino::String("This is test ... lorem ipsum lalala"));
}

TEST_CASE("Testing 'readStdStringUntil' with separator available within the stream", "[Stream-readStringUntil-03]")
{
	StreamMock mock;
	mock.setTimeout(10);
	mock << "This is test! lorem ipsum lalala";

	REQUIRE(mock.readStdStringUntil('!') == "This is test");
}

TEST_CASE("Testing 'readStdStringUntil' with separator not available within the stream", "[Stream-readStringUntil-04]")
{
	StreamMock mock;
	mock.setTimeout(10);
	mock << "This is test ... lorem ipsum lalala";

	REQUIRE(mock.readStdStringUntil('!') == "This is test ... lorem ipsum lalala");
}
