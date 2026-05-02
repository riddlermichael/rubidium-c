#pragma once

#include <rbc/core/compiler.h>
#include <rbc/core/helpers.h>

#if defined(RBC_COMPILER_CLANG)
	#define RBC_WARNING_PUSH _Pragma("clang diagnostic push")
	#define RBC_WARNING_POP _Pragma("clang diagnostic pop")
#elif defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_WARNING_PUSH _Pragma("GCC diagnostic push")
	#define RBC_WARNING_POP _Pragma("GCC diagnostic pop")
#elif defined(RBC_COMPILER_MSVC)
	#define RBC_WARNING_PUSH __pragma(warning(push))
	#define RBC_WARNING_POP __pragma(warning(pop))
#endif

#if defined(RBC_COMPILER_CLANG)
	#define RBC_WARNING_DEPRECATED \
		_Pragma(RBC_STRINGIFY(clang diagnostic ignored "-Wdeprecated-declarations"))
	#define RBC_WARNING_POSSIBLE_NULL_ARGUMENT
	#define RBC_WARNING_POSSIBLE_NULL_DEREFERENCE
	#define RBC_WARNING_ENUM_SWITCH
	#define RBC_WARNING_PADDING \
		_Pragma(RBC_STRINGIFY(clang diagnostic ignored "-Wpadded"))
#elif defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_WARNING_DEPRECATED \
		_Pragma(RBC_STRINGIFY(GCC diagnostic ignored "-Wdeprecated-declarations"))
	#define RBC_WARNING_POSSIBLE_NULL_ARGUMENT \
		_Pragma(RBC_STRINGIFY(GCC diagnostic ignored "-Wanalyzer-possible-null-argument"))
	#define RBC_WARNING_POSSIBLE_NULL_DEREFERENCE \
		_Pragma(RBC_STRINGIFY(GCC diagnostic ignored "-Wanalyzer-possible-null-dereference"))
	#define RBC_WARNING_ENUM_SWITCH
	#define RBC_WARNING_PADDING \
		_Pragma(RBC_STRINGIFY(GCC diagnostic ignored "-Wpadded"))
#elif defined(RBC_COMPILER_MSVC)
	#define RBC_WARNING_DEPRECATED \
		__pragma(warning(disable : 5039))
	#define RBC_WARNING_POSSIBLE_NULL_ARGUMENT
	#define RBC_WARNING_POSSIBLE_NULL_DEREFERENCE \
		__pragma(warning(disable : 6011))
	#define RBC_WARNING_ENUM_SWITCH \
		__pragma(warning(disable : 4061))
	#define RBC_WARNING_PADDING \
		__pragma(warning(disable : 4820))
#endif
