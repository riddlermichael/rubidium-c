#pragma once

#include <time.h>

#include <rbc/core/attributes.h>
#include <rbc/core/extern.h>
#include <rbc/core/keywords.h>

enum {
	RBC_NANOSECONDS_PER_SECOND = 1000000000,
};

/// Structure holding an interval broken down into seconds and nanoseconds.
/// Replacement for `timespec` in C11.
struct RbcTimespec {
	time_t tv_sec;
	long tv_nsec;
};
typedef struct RbcTimespec RbcTimespec;

RBC_BEGIN_EXTERN_C

typedef struct timespec StdTimespec;

RBC_NOTHROW RBC_CONST StdTimespec rbc_timespec_to_std_timespec(RbcTimespec self);
RBC_NOTHROW RBC_CONST RbcTimespec rbc_timespec_from_std_timespec(StdTimespec ts);

/**
 * Return a RbcTimespec object holds the current calendar time.
 * `tv_sec` is set to the number of seconds since the Epoch, truncated to a whole value.
 * `tv_nsec` is set to the integral number of nanoseconds, rounded to the resolution of the system clock.
 */
RBC_NOTHROW RbcTimespec rbc_timespec_get(void);

/**
 * Return the resolution of time provided by `timespec_get`.
 * Multiple calls to `timespec_getres` during the same program execution have identical results.
 */
RBC_NOTHROW RbcTimespec rbc_timespec_resolution(void);

RBC_END_EXTERN_C
