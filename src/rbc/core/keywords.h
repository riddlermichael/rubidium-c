#pragma once

#include <stdnoreturn.h>

#include <rbc/core/compiler.h>

#ifdef __cplusplus
	#define RBC_ALIGNAS(x) alignas(x)
	#define RBC_ALIGNOF(x) alignof(x)
	#define RBC_NORETURN [[noreturn]]
	#define RBC_RESTRICT __restrict
	#define RBC_STATIC_ASSERT(cond) static_assert(cond, #cond)
	#define RBC_STATIC_ASSERT_X(cond, msg) static_assert(cond, msg)
	#define RBC_THREAD_LOCAL thread_local
#else
	#define RBC_ALIGNAS(x) _Alignas(x)
	#define RBC_ALIGNOF(x) _Alignof(x)
	#ifdef noreturn
		#define RBC_NORETURN noreturn
	#elif defined(RBC_COMPILER_MSVC)
		#define RBC_NORETURN __declspec(noreturn)
	#else
		#define RBC_NORETURN
	#endif
	#define RBC_RESTRICT restrict
	#define RBC_STATIC_ASSERT(cond) _Static_assert(cond, #cond)
	#define RBC_STATIC_ASSERT_X(cond, msg) _Static_assert(cond, msg)

	#if defined(RBC_COMPILER_GCC_LIKE)
		#define RBC_THREAD_LOCAL _Thread_local
	#elif defined(RBC_COMPILER_MSVC)
		#define RBC_THREAD_LOCAL __declspec(thread)
	#endif
#endif

#if defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_NOTHROW __attribute__((nothrow))
#elif defined(RBC_COMPILER_MSVC) && defined(__cplusplus)
	#define RBC_NOTHROW __declspec(nothrow)
#else
	#define RBC_NOTHROW
#endif
