#pragma once

#include <stdio.h>
#ifdef __cplusplus
	#include <ostream>
#endif

#include <rbc/core/builtins.h>
#include <rbc/core/export.h>
#include <rbc/core/extern.h>
#include <rbc/core/features.h>

struct rbc_source_location {
	char const* file;
	char const* func; // unnatural order here due to alignment
	unsigned line;
#if RBC_HAS(BUILTIN_COLUMN)
	unsigned column;
#endif
};

typedef struct rbc_source_location rbc_source_location;

#ifdef __cplusplus

// designated initializers are not supported until C++20, so we need a plain function
	#define RBC_SOURCE_LOC rbc_source_location_current(RBC_BUILTIN_FILE, RBC_BUILTIN_LINE, RBC_BUILTIN_FUNCTION, RBC_BUILTIN_COLUMN)
	#define RBC_SOURCE_LOC_FN rbc_source_location_current(RBC_BUILTIN_FILE, RBC_BUILTIN_LINE, RBC_CURRENT_FUNCTION, RBC_BUILTIN_COLUMN)

inline std::ostream& operator<<(std::ostream& os, rbc_source_location loc) {
	os << loc.file << ":" << loc.line;
	#if RBC_ENABLED(FULL_SOURCE_LOCATION_INFO)
		#if RBC_HAS(BUILTIN_COLUMN)
	os << ":" << loc.column;
		#endif
	os << " (" << loc.func << ")";
	#endif
	return os;
}

#else
	#if RBC_HAS(BUILTIN_COLUMN)
		#define RBC_SOURCE_LOC                \
			(rbc_source_location) {           \
				.file = RBC_BUILTIN_FILE,     \
				.func = RBC_BUILTIN_FUNCTION, \
				.line = RBC_BUILTIN_LINE,     \
				.column = RBC_BUILTIN_COLUMN  \
			}

	// Macros used in RBC_CURRENT_FUNCTION are defined only in functions and caused compilation error on MSVC if used outside
		#define RBC_SOURCE_LOC_FN             \
			(rbc_source_location) {           \
				.file = RBC_BUILTIN_FILE,     \
				.func = RBC_CURRENT_FUNCTION, \
				.line = RBC_BUILTIN_LINE,     \
				.column = RBC_BUILTIN_COLUMN  \
			}
	#else
		#define RBC_SOURCE_LOC                \
			(rbc_source_location) {           \
				.file = RBC_BUILTIN_FILE,     \
				.func = RBC_BUILTIN_FUNCTION, \
				.line = RBC_BUILTIN_LINE      \
			}

		#define RBC_SOURCE_LOC_FN             \
			(rbc_source_location) {           \
				.file = RBC_BUILTIN_FILE,     \
				.func = RBC_CURRENT_FUNCTION, \
				.line = RBC_BUILTIN_LINE      \
			}
	#endif
#endif

RBC_BEGIN_EXTERN_C

RBC_EXPORT void rbc_source_location_print(rbc_source_location loc);
RBC_EXPORT void rbc_source_location_print_to(rbc_source_location loc, FILE* f);
RBC_EXPORT RBC_NOTHROW rbc_source_location rbc_source_location_current(
    char const* file,
    unsigned line,
    char const* func,
    unsigned column);

RBC_END_EXTERN_C
