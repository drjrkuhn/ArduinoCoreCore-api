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

TEST_CASE ("Testing readBytes(char *buffer, size_t length)", "[Stream-readBytes-01]")
{
  StreamMock mock;
  mock.setTimeout(10);

  WHEN ("the stream is empty")
  {
    char buf[32] = {0};

    REQUIRE(mock.readBytes(buf, sizeof(buf)) == 0);
  }

  WHEN ("the stream contains less data than we want to read")
  {
    char buf[32] = {0};
    char const str[] = "some stream content";
    mock << str;

    REQUIRE(mock.readBytes(buf, sizeof(buf)) == strlen(str));
    REQUIRE(strncmp(buf, str, sizeof(buf)) == 0);
    REQUIRE(mock.readString() == arduino::String(""));
  }

  WHEN ("the stream contains more data than we want to read")
  {
    char buf[5] = {0};
    mock << "some stream content";
    char const EXPECTED_STR[] = "some ";

    REQUIRE(mock.readBytes(buf, sizeof(buf)) == 5);
    REQUIRE(strncmp(buf, EXPECTED_STR, sizeof(buf)) == 0);
    REQUIRE(mock.readString() == arduino::String("stream content"));
  }
}

TEST_CASE("Testing readBytes_for(iterator begin, iterator end)", "[Stream-readBytes-02]")
{
    StreamMock mock;
    mock.setTimeout(10);

    WHEN("the stream is empty")
    {
        std::string buf(32, '\0');

        REQUIRE(mock.readBytes_to(buf.begin(), buf.end()) == 0);
    }

    WHEN("the stream contains less data than we want to read")
    {
        std::string buf(32, '\0');
        char const str[] = "some stream content";
        mock << str;

        REQUIRE(mock.readBytes_to(buf.begin(), buf.end()) == strlen(str));
        REQUIRE(strncmp(buf.c_str(), str, 32) == 0);
        REQUIRE(mock.readStdString() == "");
    }

    WHEN("the stream contains more data than we want to read")
    {
        std::string buf(5, '\0');
        mock << "some stream content";
        char const EXPECTED_STR[] = "some ";

        REQUIRE(mock.readBytes_to(buf.begin(), buf.end()) == 5);
        REQUIRE(strncmp(buf.c_str(), EXPECTED_STR, 5) == 0);
        REQUIRE(mock.readStdString() == "stream content");
    }
}
