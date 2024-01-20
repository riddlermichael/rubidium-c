#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <rbc/core/source_location.h>

using Catch::Matchers::EndsWith;
using Catch::Matchers::Equals;

TEST_CASE("current", "[source_location]") {
	auto const line = __LINE__ + 1;
	auto const loc = RBC_SOURCE_LOC;
	REQUIRE_THAT(loc.file, EndsWith("source_location.cpp"));
	REQUIRE(loc.line == line);
	REQUIRE_THAT(loc.func, Equals("CATCH2_INTERNAL_TEST_0"));
}
