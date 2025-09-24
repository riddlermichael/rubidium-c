#pragma once

#if defined(_MSC_VER)
	#define RBC_COMPILER_MSVC
	#ifdef __clang__
		#define RBC_COMPILER_CLANG
	#endif
	#define RBC_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
	#define RBC_COMPILER_VERSION_MINOR (_MSC_VER % 100)
	#ifdef _MSC_FULL_VER
		#if _MSC_FULL_VER / 10000 == _MSC_VER
			#define RBC_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 10000)
		#elif _MSC_FULL_VER / 100000 == _MSC_VER
			#define RBC_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
		#else
			#define RBC_COMPILER_VERSION_PATCH 0
		#endif
	#else
		#define RBC_COMPILER_VERSION_PATCH 0
	#endif
#elif defined(__GNUC__)
	#define RBC_COMPILER_GCC_LIKE
	#ifdef __MINGW32__
		#define RBC_COMPILER_MINGW
	#elif defined(__clang__)
		#define RBC_COMPILER_CLANG
	#else
		#define RBC_COMPILER_GCC
	#endif
	#ifdef RBC_COMPILER_CLANG
		#define RBC_COMPILER_VERSION_MAJOR __clang_major__
		#define RBC_COMPILER_VERSION_MINOR __clang_minor__
		#define RBC_COMPILER_VERSION_PATCH __clang_patchlevel__
		#define RBC_COMPILER_VERSION_STRING __clang_version__
	#else
		#define RBC_COMPILER_VERSION_MAJOR __GNUC__
		#define RBC_COMPILER_VERSION_MINOR __GNUC_MINOR__
		#ifdef __GNUC_PATCHLEVEL__
			#define RBC_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
		#else
			#define RBC_COMPILER_VERSION_PATCH 0
		#endif
		#define RBC_COMPILER_VERSION_STRING __VERSION__
	#endif
#else
	#error Unsupported compiler
#endif

enum rbc_compiler_id {
	RBC_COMPILER_ID_CLANG = 0,
	RBC_COMPILER_ID_GCC = 1,
	RBC_COMPILER_ID_MINGW = 2,
	RBC_COMPILER_ID_MSVC = 3,
#if defined(RBC_COMPILER_CLANG)
	RBC_COMPILER_ID = RBC_COMPILER_ID_CLANG
#elif defined(RBC_COMPILER_GCC)
	RBC_COMPILER_ID = RBC_COMPILER_ID_GCC
#elif defined(RBC_COMPILER_MINGW)
	RBC_COMPILER_ID = RBC_COMPILER_ID_MINGW
#elif defined(RBC_COMPILER_MSVC)
	RBC_COMPILER_ID = RBC_COMPILER_ID_MSVC
#endif
};
typedef enum rbc_compiler_id rbc_compiler_id;
