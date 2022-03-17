#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "StringHelpers.h"

#define SU_CHARACTERS			char, unsigned char
#define SU_SIGNED_INTEGRALS		short, int, long, long long
#define SU_UNSIGNED_INTEGRALS	unsigned short, unsigned int, unsigned long, unsigned long long
#define SU_INTEGRALS			SU_SIGNED_INTEGRALS, SU_UNSIGNED_INTEGRALS
#define SU_FLOATS				float, double, long double

#define DEC	10
#define HEX 16
#define OCT 8

TEST_SUITE("[String_helpers.h]") {
	using namespace arduino;

	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_CHARACTERS) {
		CHECK(to_string<N>(N(100), DEC) == "100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_INTEGRALS) {
		CHECK(to_string<N>(N(100), DEC) == "100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_SIGNED_INTEGRALS) {
		CHECK(to_string<N>(N(-100), DEC) == "-100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring", N, SU_FLOATS) {
		CHECK(to_string<N>(N(100.0), 2) == "100.00");
		CHECK(to_string<N>(N(-100.0), 2) == "-100.00");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring DEC", N, SU_INTEGRALS) {
		CHECK(to_string<N>(N(100), DEC) == "100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring HEX", N, SU_INTEGRALS) {
		CHECK(to_string<N>(N(100), HEX) == "64");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring OCT", N, SU_INTEGRALS) {
		CHECK(to_string<N>(N(100), OCT) == "144");
	}

	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring signed DEC", N, SU_SIGNED_INTEGRALS) {
		CHECK(to_string<N>(N(-100), DEC) == "-100");
	}
	TEST_CASE_TEMPLATE("[String_helpers.h] to_stdstring float 4 places", N, SU_FLOATS) {
		CHECK(to_string<N>(N(100.0), 4) == "100.0000");
		CHECK(to_string<N>(N(-100.0), 4) == "-100.0000");
	}
}
