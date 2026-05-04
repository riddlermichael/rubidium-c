#pragma once

#include <limits.h>

#include <rbc/core/compiler.h>

#define RBC_I8_MIN (-127 - 1)
#define RBC_I8_MAX 127
#define RBC_I16_MIN (-32767 - 1)
#define RBC_I16_MAX 32767
#define RBC_I32_MIN (-2147483647 - 1)
#define RBC_I32_MAX 2147483647
#define RBC_I64_MAX 9223372036854775807LL
#define RBC_I64_MIN (-RBC_I64_MAX - 1LL)

#define RBC_U8_MIN 0U
#define RBC_U8_MAX 255U
#define RBC_U16_MIN 0U
#define RBC_U16_MAX 65535U
#define RBC_U32_MIN 0U
#define RBC_U32_MAX 4294967295U
#define RBC_U64_MIN 0U
#define RBC_U64_MAX 18446744073709551615ULL

#define RBC_ISIZE_MIN INTPTR_MIN
#define RBC_ISIZE_MAX INTPTR_MAX
#define RBC_USIZE_MIN 0
#define RBC_USIZE_MAX UINTPTR_MAX

#define RBC_F32_MIN 0x1p-126F
#define RBC_F32_MAX 0x1.fffffep127F
#define RBC_F32_INF __builtin_huge_valf()
#define RBC_F64_MIN 0x1p-1022
#define RBC_F64_MAX 0x1.fffffffffffffp1023
#define RBC_F64_INF __builtin_huge_val()

#ifndef __cplusplus

	#ifdef RBC_COMPILER_MSVC
		#if CHAR_MIN == SCHAR_MIN
			#define RBC_BRANCH_SCHAR(X)
			#define RBC_BRANCH_UCHAR(X) X,
		#else
			#define RBC_BRANCH_SCHAR(X) X,
			#define RBC_BRANCH_UCHAR(X)
		#endif
	#else
		#define RBC_BRANCH_SCHAR(X) X,
		#define RBC_BRANCH_UCHAR(X) X,
	#endif

// clang-format off
	#define RBC_MIN(T)                                \
		_Generic((T) 0,                               \
			char: CHAR_MIN,                           \
			RBC_BRANCH_SCHAR(signed char : SCHAR_MIN) \
		    RBC_BRANCH_UCHAR(unsigned char : 0)       \
		    short: SHRT_MIN,                          \
		    int: INT_MIN,                             \
		    long: LONG_MIN,                           \
		    long long: LLONG_MIN,                     \
		    unsigned short: 0,                        \
		    unsigned int: 0,                          \
		    unsigned long: 0,                         \
		    unsigned long long: 0,                    \
		    float: RBC_F32_MIN,                       \
		    double: RBC_F64_MIN)

	#define RBC_MAX(T)                                  \
		_Generic((T) 0,                                 \
		    char: CHAR_MAX,                             \
		    RBC_BRANCH_SCHAR(signed char : SCHAR_MAX)   \
		    RBC_BRANCH_UCHAR(unsigned char : UCHAR_MAX) \
		    short: SHRT_MAX,                            \
		    int: INT_MAX,                               \
		    long: LONG_MAX,                             \
		    long long: LLONG_MAX,                       \
		    unsigned short: USHRT_MAX,                  \
		    unsigned int: UINT_MAX,                     \
		    unsigned long: ULONG_MAX,                   \
		    unsigned long long: ULLONG_MAX,             \
		    float: RBC_F32_MAX,                         \
		    double: RBC_F64_MAX)
// clang-format on
#endif
