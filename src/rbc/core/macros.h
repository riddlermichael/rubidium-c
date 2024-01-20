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

} // namespace

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

#ifdef RBC_COMPILER_MSVC
	#define RBC_NARGS(...) RBC_INTERNAL_EXPAND_ARGS_PRIVATE(RBC_INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))
	#define RBC_INTERNAL_ARGS_AUGMENTER(...) _UNUSED_, __VA_ARGS__
	#define RBC_INTERNAL_EXPAND(x) x
	#define RBC_INTERNAL_EXPAND_ARGS_PRIVATE(...)           \
		RBC_INTERNAL_EXPAND(                                \
		    RBC_INTERNAL_GET_ARG_COUNT_PRIVATE(__VA_ARGS__, \
		        69, 68, 67, 66, 65, 64, 63, 62, 61, 60,     \
		        59, 58, 57, 56, 55, 54, 53, 52, 51, 50,     \
		        49, 48, 47, 46, 45, 44, 43, 42, 41, 40,     \
		        39, 38, 37, 36, 35, 34, 33, 32, 31, 30,     \
		        29, 28, 27, 26, 25, 24, 23, 22, 21, 20,     \
		        19, 18, 17, 16, 15, 14, 13, 12, 11, 10,     \
		        9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
	#define RBC_INTERNAL_GET_ARG_COUNT_PRIVATE(                     \
	    _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_,          \
	    _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, \
	    _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, \
	    _31_, _32_, _33_, _34_, _35_, _36_, _37_, _38_, _39_, _40_, \
	    _41_, _42_, _43_, _44_, _45_, _46_, _47_, _48_, _49_, _50_, \
	    _51_, _52_, _53_, _54_, _55_, _56_, _57_, _58_, _59_, _60_, \
	    _61_, _62_, _63_, _64_, _65_, _66_, _67_, _68_, _69_, _70_, \
	    count, ...) count
#else
	#define RBC_NARGS(...) (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#endif
