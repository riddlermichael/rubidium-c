#pragma once

#include <time.h>

#include <rbc/core/attributes.h>
#include <rbc/core/extern.h>
#include <rbc/core/keywords.h>

/// Structure holding an interval broken down into seconds and nanoseconds.
/// Replacement for `timespec` in C11.
struct rbc_timespec {
	time_t tv_sec;
	long tv_nsec;
};

typedef struct rbc_timespec rbc_timespec;

RBC_BEGIN_EXTERN_C

typedef struct timespec std_timespec; // NOLINT(readability-identifier-naming)

RBC_NOTHROW RBC_CONST std_timespec rbc_timespec_to_std_timespec(rbc_timespec self);
RBC_NOTHROW RBC_CONST rbc_timespec rbc_timespec_from_std_timespec(std_timespec ts);

/**
 * Return a rbc_timespec object holds the current calendar time.
 * `tv_sec` is set to the number of seconds since the Epoch, truncated to a whole value.
 * `tv_nsec` is set to the integral number of nanoseconds, rounded to the resolution of the system clock.
 */
RBC_NOTHROW rbc_timespec rbc_timespec_get(void);

/**
 * Return the resolution of time provided by `timespec_get`.
 * Multiple calls to `timespec_getres` during the same program execution have identical results.
 */
RBC_NOTHROW rbc_timespec rbc_timespec_getres(void);

RBC_END_EXTERN_C
