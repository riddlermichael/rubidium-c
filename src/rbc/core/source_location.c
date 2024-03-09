#include "source_location.h"

#include <rbc/core/helpers.h>

void rbc_source_location_print(rbc_source_location loc) {
	rbc_source_location_print_to(loc, stdout);
}

// Same as `operator<<`, but there is no relation between FILE* and std::ostream&
void rbc_source_location_print_to(rbc_source_location loc, FILE* f) {
	(void) fprintf(f, "%s:%u", loc.file, loc.line);
#if RBC_ENABLED(FULL_SOURCE_LOCATION_INFO)
	#if RBC_HAS(BUILTIN_COLUMN) // NOLINT(*-redundant-preprocessor)
	(void) fprintf(f, ":%u", loc.column);
	#endif
	(void) fprintf(f, " (%s)", loc.func);
#endif
}

rbc_source_location rbc_source_location_current(char const* file, unsigned line, char const* func, unsigned column) {
	RBC_UNUSED(column);
	return (rbc_source_location){.file = file, .func = func, .line = line
#if RBC_HAS(BUILTIN_COLUMN)
	    ,
	    .column = column
#endif
	};
}
