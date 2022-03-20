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

TEST_CASE ("Testing find(const char *target)", "[Stream-find-01]")
{
  StreamMock mock;
  mock.setTimeout(10);

  WHEN ("'target' is contained in stream")
  {
    mock << "This is a test string";

    REQUIRE(mock.find("test") == true);
    REQUIRE(mock.readString() == arduino::String(" string"));
  }
  WHEN ("'target' is not contained in stream")
  {
    mock << "This is a string";

    REQUIRE(mock.find("test") == false);
    REQUIRE(mock.readString() == arduino::String(""));
  }
}

TEST_CASE ("Testing find(const char *target, size_t length)", "[Stream-find-02]")
{
  StreamMock mock;
  mock.setTimeout(10);

  WHEN ("'target' is contained in stream")
  {
    mock << "teis is a test string";

    /* 'length' should actually be '4' or strlen("test"). I'd rather
     * think this API should not be exposed at all.
     */
    REQUIRE(mock.find("test", 3) == true);
    REQUIRE(mock.readString() == arduino::String("t string"));
  }
  WHEN ("'target' is not contained in stream")
  {
    mock << "This is a string";

    REQUIRE(mock.find("test", 3) == false);
    REQUIRE(mock.readString() == arduino::String(""));
  }
}

TEST_CASE ("Testing find(char target)", "[Stream-find-03]")
{
  StreamMock mock;
  mock.setTimeout(10);

  WHEN ("'target' is contained in stream")
  {
    mock << "This is a test string";

    REQUIRE(mock.find('t') == true);
    REQUIRE(mock.readString() == arduino::String("est string"));
  }
  WHEN ("'target' is not contained in stream")
  {
    mock << "This is a string";

    REQUIRE(mock.find('!') == false);
    REQUIRE(mock.readString() == arduino::String(""));
  }
}

TEST_CASE("Testing find(std::string target)", "[Stream-find-04]")
{
    StreamMock mock;
    mock.setTimeout(10);

    WHEN("'target' is contained in stream")
    {
        mock << "This is a test string";

        REQUIRE(mock.find(std::string("test")) == true);
        REQUIRE(mock.readStdString() == " string");
    }
    WHEN("'target' is not contained in stream")
    {
        mock << "This is a string";

        REQUIRE(mock.find(std::string("test")) == false);
        REQUIRE(mock.readStdString() == "");
    }
}

