/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <IPAddress.h>
#include <Print_std.h>

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Print IPAddress via print method", "[IPAddress-printTo-01]")
{
  arduino::Print_stdstring mock;
  arduino::IPAddress const ip(192,168,1,2);

  mock.print(ip);

  REQUIRE(mock.str()  == "192.168.1.2");
}

TEST_CASE("Print IPAddress via operator<<", "[IPAddress-printTo-02]")
{
	arduino::IPAddress const ip(192, 168, 1, 2);
	std::stringstream os;
	os << ip;
	REQUIRE(os.str() == "192.168.1.2");
}
