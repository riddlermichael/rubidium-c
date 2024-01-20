#pragma once

#include <rbc/core/compiler.h>

// Generic helper definitions for shared library support
#if defined(RBC_COMPILER_MSVC)
	#define RBC_DECL_IMPORT __declspec(dllimport)
	#define RBC_DECL_EXPORT __declspec(dllexport)
	#define RBC_DECL_HIDDEN
#elif defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_DECL_IMPORT __attribute__((visibility("default")))
	#define RBC_DECL_EXPORT __attribute__((visibility("default")))
	#define RBC_DECL_HIDDEN __attribute__((visibility("hidden")))
#else
	#define RBC_DECL_IMPORT
	#define RBC_DECL_EXPORT
	#define RBC_DECL_HIDDEN
#endif

// Now we use the generic helper definitions above to define RBC_EXPORT and RBC_HIDDEN.
// RBC_EXPORT is used for the public API symbols;
// it either DLL imports or DLL exports (or does nothing for static build).
// RBC_HIDDEN is used for non-api symbols.
#ifdef RBC_STATIC
	#define RBC_EXPORT
	#define RBC_HIDDEN
#else
	#ifndef RBC_EXPORT
		#ifdef RBC_EXPORTS
			#define RBC_EXPORT RBC_DECL_EXPORT
		#else
			#define RBC_EXPORT RBC_DECL_IMPORT
		#endif
	#endif
	#define RBC_HIDDEN RBC_DECL_HIDDEN
#endif
