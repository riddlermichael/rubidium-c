#include "sanitizers.h"

#include <rbc/core/attributes.h>

#if RBC_ENABLED(ADDRESS_SANITIZER)
// no_sanitize_address on GCC simply doesn't work if we have invalid pointer pairs; see fmt::toChars
RBC_MAYBE_UNUSED char const* __asan_default_options(void) { // NOLINT
	return "check_initialization_order=1"
	       ":detect_stack_use_after_return=1"
	       ":strict_init_order=1"
	       ":strict_string_checks=1"
	#ifndef RBC_COMPILER_GCC
	       ":detect_invalid_pointer_pairs=2"
	#endif
	#ifdef RBC_COMPILER_MSVC
	       ":windows_hook_rtl_allocators=1"
	#endif
	    ;
}
#endif
