
/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

 /**************************************************************************************
  * INCLUDE
  **************************************************************************************/

#include <catch.hpp>

#include <WString.h>

#include <utility>

/**************************************************************************************
* TEST CODE
**************************************************************************************/

TEST_CASE("Testing String move constructor", "[String-move-01]")
{
    arduino::String a("src");
    arduino::String b(std::move(a));
    REQUIRE(a.length() == 0);
    REQUIRE(!a); //REQUIRE(a.c_str() == nullptr);
    REQUIRE(b.length() == 3);
}

TEST_CASE("Testing String move assignment", "[String-move-02]")
{
    arduino::String a("src");
    arduino::String b;
    b = std::move(a);
    REQUIRE(a.length() == 0);
    REQUIRE(!a); // REQUIRE(a.c_str() == nullptr);
    REQUIRE(b == arduino::String("src"));
}

TEST_CASE("Testing String move self assignment", "[String-move-03]")
{
    arduino::String a("src");
    a = std::move(a);
    REQUIRE(a == "src");
}
