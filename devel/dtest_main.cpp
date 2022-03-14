#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/* PULL TESTS from headers */
#define DOCTEST_STRING_HELPERS
#define DOCTEST_NEW_STRING
#define DOCTEST_NEW_PRINT
// #define DOCTEST_NEW_STREAM

#include "String_helpers.h"
#include "new_String.h"
#include "new_Print.h"
// #include "new_Stream.h"

TEST_CASE("[doctest_main.cpp]") {
	//MESSAGE("doctest_main.cpp");
}
