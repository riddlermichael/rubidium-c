#pragma once

#include <rbc/core/compiler.h>
#include <rbc/core/features.h>

#if defined(__has_feature) // clang
	#if __has_feature(thread_sanitizer)
		#define RBC_FEATURE_THREAD_SANITIZER 1
	#else
		#define RBC_FEATURE_THREAD_SANITIZER (-1)
	#endif
#elif defined(__SANITIZE_THREAD__) // gcc
	#define RBC_FEATURE_THREAD_SANITIZER 1
#else
	#define RBC_FEATURE_THREAD_SANITIZER (-1)
#endif

#if defined(__has_feature) // clang
	#if __has_feature(address_sanitizer)
		#define RBC_FEATURE_ADDRESS_SANITIZER 1
	#else
		#define RBC_FEATURE_ADDRESS_SANITIZER (-1)
	#endif
#elif defined(__SANITIZE_ADDRESS__) // gcc
	#define RBC_FEATURE_ADDRESS_SANITIZER 1
#else
	#define RBC_FEATURE_ADDRESS_SANITIZER (-1)
#endif

#if defined(__has_feature) // clang
	#if __has_feature(memory_sanitizer)
		#define RBC_FEATURE_MEMORY_SANITIZER 1
	#else
		#define RBC_FEATURE_MEMORY_SANITIZER (-1)
	#endif
#else // MSAN unsupported by GCC
	#define RBC_FEATURE_MEMORY_SANITIZER (-1)
#endif

#if defined(__has_feature) // clang
	#if __has_feature(undefined_behavior_sanitizer)
		#define RBC_FEATURE_UB_SANITIZER 1
	#else
		#define RBC_FEATURE_UB_SANITIZER (-1)
	#endif
#elif defined(__SANITIZE_UNDEFINED__) // pick up from CMake
	#define RBC_FEATURE_UB_SANITIZER 1
#else
	#define RBC_FEATURE_UB_SANITIZER (-1)
#endif

#ifdef RBC_COMPILER_GCC_LIKE
	#define RBC_ATTR_NO_SANITIZE(check) __attribute__((no_sanitize(check)))
#else
	#define RBC_ATTR_NO_SANITIZE(check)
#endif

#ifdef RBC_COMPILER_CLANG
	#define RBC_ATTR_NO_SANITIZE_UNSIGNED_INTEGER_OVERFLOW __attribute__((no_sanitize("unsigned-integer-overflow")))
#else
	#define RBC_ATTR_NO_SANITIZE_UNSIGNED_INTEGER_OVERFLOW
#endif
