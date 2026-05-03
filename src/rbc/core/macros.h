#pragma once

#include <rbc/core/keywords.h>

#define RBC_CONCAT_NX(A, B) A##B
#define RBC_CONCAT(A, B) RBC_CONCAT_NX(A, B)

#ifdef __cplusplus

namespace rbc {

template <class T, class U>
struct IsSame {
	enum {
		value = false
	};
};

template <class T>
struct IsSame<T, T> {
	enum {
		value = true
	};
};

} // namespace rbc

	#define RBC_TYPES_COMPATIBLE_EXPR(lhs, rhs) rbc::IsSame<decltype(lhs), decltype(rhs)>::value

#elif defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_TYPES_COMPATIBLE_EXPR(lhs, rhs) \
		__builtin_types_compatible_p(__typeof__(lhs), __typeof__(rhs))
#endif

#if defined(RBC_COMPILER_GCC_LIKE) && !defined(__cplusplus)
	#define RBC_IS_ARRAY(a) \
		(!RBC_TYPES_COMPATIBLE_EXPR((a), &(a)[0]))
	#define RBC_ARRAY_SIZE(a) \
		({ RBC_STATIC_ASSERT_X(RBC_IS_ARRAY(a), "Argument is a non-array"); sizeof(a) / sizeof((a)[0]); })
#else
	#define RBC_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#define RBC_INTERNAL_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

#define RBC_NARGS(...) \
	RBC_INTERNAL_EXPAND(RBC_INTERNAL_ARG_N(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

// need for MSVC
#define RBC_INTERNAL_EXPAND(x) x
