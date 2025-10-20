#include "timespec.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <rbc/core/os.h>
#include <rbc/core/types.h>

std_timespec rbc_timespec_to_std_timespec(rbc_timespec self) {
	return (std_timespec) {.tv_sec = self.tv_sec, .tv_nsec = self.tv_nsec};
}

rbc_timespec rbc_timespec_from_std_timespec(std_timespec ts) {
	return (rbc_timespec) {.tv_sec = ts.tv_sec, .tv_nsec = ts.tv_nsec};
}

#ifdef RBC_OS_WIN

	#include <Windows.h>

	#define RBC_WIN_TICKS_FROM_1601_TO_UNIX_EPOCH 116444736000000000ULL
	#define RBC_WIN_TICKS_PER_SECOND 10000000U
	#define RBC_NANOSECONDS_PER_WIN_TICK 100U

rbc_timespec rbc_timespec_get(void) {
	FILETIME ft;
	GetSystemTimePreciseAsFileTime(&ft);
	// QuadPart is # of 100ns ticks since 1601-01-01T00:00:00Z
	u64 const ticks_since_1601 = (ULARGE_INTEGER) {.LowPart = ft.dwLowDateTime, .HighPart = ft.dwHighDateTime}.QuadPart;
	u64 const ticks = ticks_since_1601 - RBC_WIN_TICKS_FROM_1601_TO_UNIX_EPOCH;
	u64 const secs = ticks / RBC_WIN_TICKS_PER_SECOND;
	u64 const nsecs = (ticks - secs * RBC_WIN_TICKS_PER_SECOND) * RBC_NANOSECONDS_PER_WIN_TICK;
	return (rbc_timespec) {secs, nsecs};
}

rbc_timespec rbc_timespec_getres(void) {
	LARGE_INTEGER freq;
	#if _WIN32_WINNT >= 0x0600
	QueryPerformanceFrequency(&freq);
	#else
	if (!QueryPerformanceFrequency(&freq)) {
		freq.QuadPart = RBC_WIN_TICKS_PER_SECOND;
	}
	#endif

	rbc_timespec ts = {0};
	ts.tv_nsec = (long) ((RBC_NANOSECONDS_PER_SECOND + (freq.QuadPart / 2)) / freq.QuadPart);
	return ts;
}

	#undef RBC_NANOSECONDS_PER_WIN_TICK
	#undef RBC_WIN_TICKS_PER_SECOND
	#undef RBC_WIN_TICKS_FROM_1601_TO_UNIX_EPOCH

#else

rbc_timespec rbc_timespec_get(void) {
	std_timespec ts;
	(void) clock_gettime(CLOCK_REALTIME, &ts);
	return rbc_timespec_from_std_timespec(ts);
}

rbc_timespec rbc_timespec_getres(void) {
	std_timespec ts;
	(void) clock_getres(CLOCK_REALTIME, &ts);
	return rbc_timespec_from_std_timespec(ts);
}

#endif
