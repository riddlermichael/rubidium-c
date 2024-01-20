#pragma once

#include <rbc/core/source_location.h>

RBC_BEGIN_EXTERN_C

// Should have extern "C" linkage
typedef void (*rbc_assert_handler)(char const* msg, rbc_source_location loc);

/// **Do not use in production code, use RBC_ASSERT macro instead.**
RBC_NORETURN RBC_EXPORT void rbc_throw_assert(char const* msg, rbc_source_location loc);
/// **Do not use in production code, use RBC_ASSERT_X macro instead.**
RBC_NORETURN RBC_EXPORT void rbc_throw_assert_x(char const* msg, rbc_source_location loc, char const* fmt, ...);

RBC_EXPORT RBC_NOTHROW rbc_assert_handler rbc_get_assert_handler(void);
RBC_EXPORT RBC_NOTHROW rbc_assert_handler rbc_set_assert_handler(rbc_assert_handler handler);

RBC_END_EXTERN_C

/** Our own assert macro.
 *
 * #RBC_ASSERT checks if its argument (which must have scalar type) compares equal to zero.
 * If it does, #RBC_ASSERT outputs diagnostic information to the log
 * and calls `exit()` (unlike `std::assert`, which calls `abort()`).
 */
#define RBC_ASSERT(cond) (cond) ? (void) (0) : rbc_throw_assert(#cond, RBC_SOURCE_LOC_FN)

/// Extended assert with additional info.
/// Usage: @code RBC_ASSERT_X(2 + 2 == 5, "catch %d", 22) @endcode
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
#define RBC_ASSERT_X(cond, ...) (cond) ? (void) (0) : rbc_throw_assert_x(#cond, RBC_SOURCE_LOC_FN, __VA_ARGS__)

#ifdef NDEBUG
    /// Assert which enabled only in debug mode.
	#define RBC_DEBUG_ASSERT_C(cond) ((void) 0)
    /// Extended assert which enabled only in debug mode.
	#define RBC_DEBUG_ASSERT_X(cond, ...) ((void) 0)
#else
/// Assert which enabled only in debug mode.
	#define RBC_DEBUG_ASSERT_C RBC_ASSERT
/// Extended assert which enabled only in debug mode.
	#define RBC_DEBUG_ASSERT_X RBC_ASSERT_X
#endif

#define RBC_UNREACHABLE_ASSERT()                        \
	do {                                                \
		RBC_ASSERT_X(0, "RBC_UNREACHABLE was reached"); \
		RBC_UNREACHABLE();                              \
	} while (false)
