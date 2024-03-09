#pragma once

/// Used for macro's arguments.
#define RBC_STRINGIFY_X(x) RBC_STRINGIFY(x)

///
#define RBC_STRINGIFY(x) #x

/// Used to mark (possibly) unused variables.
#ifdef __cplusplus
	#define RBC_UNUSED(x) static_cast<void>(x)
#else
	#define RBC_UNUSED(x) (void) (x)
#endif
