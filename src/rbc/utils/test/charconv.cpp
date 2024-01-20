#include <catch2/catch_test_macros.hpp>

#include <rbc/utils/charconv.h>

TEST_CASE("u64max", "[charconv]") {
	char s[] = "abcdefghijklmnopqrst";
	u64 x = UINT64_MAX;
	rbc_to_chars_result result = rbc_to_chars(s, s + sizeof(s) - 1, x);
	REQUIRE(result.error == RBC_ERROR_OK);
}
