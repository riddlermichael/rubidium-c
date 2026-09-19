#include "timespec.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <rbc/core/os.h>
#include <rbc/core/types.h>

StdTimespec rbc_timespec_to_std_timespec(RbcTimespec self) {
	return (StdTimespec) {.tv_sec = self.tv_sec, .tv_nsec = self.tv_nsec};
}

RbcTimespec rbc_timespec_from_std_timespec(StdTimespec ts) {
	return (RbcTimespec) {.tv_sec = ts.tv_sec, .tv_nsec = ts.tv_nsec};
}

#ifdef RBC_OS_WIN

	#include <Windows.h>

	#define RBC_WIN_TICKS_FROM_1601_TO_UNIX_EPOCH 116444736000000000ULL
	#define RBC_WIN_TICKS_PER_SECOND 10000000U
	#define RBC_NANOSECONDS_PER_WIN_TICK 100U

RbcTimespec rbc_timespec_get(void) {
	FILETIME ft;
	GetSystemTimePreciseAsFileTime(&ft);
	// QuadPart is # of 100ns ticks since 1601-01-01T00:00:00Z
	u64 const ticks_since_1601 = (ULARGE_INTEGER) {.LowPart = ft.dwLowDateTime, .HighPart = ft.dwHighDateTime}.QuadPart;
	u64 const ticks = ticks_since_1601 - RBC_WIN_TICKS_FROM_1601_TO_UNIX_EPOCH;
	u64 const secs = ticks / RBC_WIN_TICKS_PER_SECOND;
	u64 const nsecs = (ticks - secs * RBC_WIN_TICKS_PER_SECOND) * RBC_NANOSECONDS_PER_WIN_TICK;
	return (RbcTimespec) {.tv_sec = (time_t) secs, .tv_nsec = (long) nsecs}; // TODO overflow
}

RbcTimespec rbc_timespec_resolution(void) {
	return (RbcTimespec) {.tv_sec = 0, .tv_nsec = RBC_NANOSECONDS_PER_WIN_TICK};
}

	#undef RBC_NANOSECONDS_PER_WIN_TICK
	#undef RBC_WIN_TICKS_PER_SECOND
	#undef RBC_WIN_TICKS_FROM_1601_TO_UNIX_EPOCH

#else

RbcTimespec rbc_timespec_get(void) {
	StdTimespec ts;
	(void) clock_gettime(CLOCK_REALTIME, &ts);
	return rbc_timespec_from_std_timespec(ts);
}

RbcTimespec rbc_timespec_resolution(void) {
	StdTimespec ts;
	(void) clock_getres(CLOCK_REALTIME, &ts);
	return rbc_timespec_from_std_timespec(ts);
}

#endif
