#pragma once

#include <rbc/core/keywords.h>

#ifdef __has_builtin
	#define RBC_HAS_BUILTIN(x) __has_builtin(x)
#else
	#define RBC_HAS_BUILTIN(x) 0
#endif

#if defined(RBC_COMPILER_GCC_LIKE) || defined(RBC_COMPILER_CLANG)
	#define RBC_BUILTIN_FILE __builtin_FILE()
	#define RBC_BUILTIN_LINE __builtin_LINE()
	#define RBC_BUILTIN_FUNCTION __builtin_FUNCTION()
	#ifdef RBC_COMPILER_CLANG
		#define RBC_BUILTIN_COLUMN __builtin_COLUMN()
		#define RBC_HAS_BUILTIN_COLUMN 1
	#else
		#define RBC_BUILTIN_COLUMN 0
		#define RBC_HAS_BUILTIN_COLUMN -1
	#endif
	#define RBC_BUILTIN_FUNCSIG __builtin_FUNCTION()
	#define RBC_HAS_BUILTIN_SOURCE_LOCATION 1
#elif defined(RBC_COMPILER_MSVC)
	#if defined(__cplusplus) && _MSC_VER >= 1926
		#define RBC_BUILTIN_FILE __builtin_FILE()
		#define RBC_BUILTIN_LINE __builtin_LINE()
		#define RBC_BUILTIN_FUNCTION __builtin_FUNCTION()
		#define RBC_BUILTIN_COLUMN __builtin_COLUMN()
		#define RBC_BUILTIN_FUNCSIG __builtin_FUNCSIG()
		#define RBC_HAS_BUILTIN_SOURCE_LOCATION 1
		#define RBC_HAS_BUILTIN_COLUMN 1
	#else
		#define RBC_BUILTIN_FILE __FILE__
		#define RBC_BUILTIN_LINE __LINE__
		#define RBC_BUILTIN_FUNCTION "<unknown>"
		#define RBC_BUILTIN_COLUMN 0
		#define RBC_BUILTIN_FUNCSIG __FUNCSIG__
		#define RBC_HAS_BUILTIN_SOURCE_LOCATION -1
		#define RBC_HAS_BUILTIN_COLUMN -1
	#endif
#else
	#define RBC_BUILTIN_FILE __FILE__
	#define RBC_BUILTIN_LINE __LINE__
	#define RBC_BUILTIN_FUNCTION __func__
	#define RBC_BUILTIN_COLUMN 0
	#define RBC_BUILTIN_FUNCSIG __func__
	#define RBC_HAS_BUILTIN_SOURCE_LOCATION -1
	#define RBC_HAS_BUILTIN_COLUMN -1
#endif

#if defined(RBC_COMPILER_MSVC)
	#define RBC_UNREACHABLE() __assume(0)
#elif defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_UNREACHABLE() __builtin_unreachable()
#else
RBC_NORETURN inline void rbc_unreachable_impl() {
}

	#define RBC_UNREACHABLE() (rbc_unreachable_impl())
#endif

#if defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_UNLIKELY(x) __builtin_expect(!!(x), 0)
	#define RBC_LIKELY(x) __builtin_expect(!!(x), 1)
#else
	#define RBC_UNLIKELY(x) (x)
	#define RBC_LIKELY(x) (x)
#endif

#if defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(RBC_COMPILER_MSVC)
	#define RBC_CURRENT_FUNCTION __FUNCSIG__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define RBC_CURRENT_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define RBC_CURRENT_FUNCTION __func__
#else
	#define RBC_CURRENT_FUNCTION "(unknown)"
#endif
