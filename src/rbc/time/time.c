#include "time.h"

#include <rbc/core/os.h>

rbc_time rbc_time_unix_epoch(void) {
	return RBC_TIME_UNIX_EPOCH;
}

#ifdef RBC_OS_LINUX

	#include <time.h> // NOLINT(readability-duplicate-include)

rbc_time rbc_time_now(void) {
	std_timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (rbc_time){.rep = rbc_duration_from_timespec(rbc_timespec_from_std_timespec(ts))};
}

#else

	#include <Windows.h>

	#define RBC_TICKS_FROM_1601_TO_UNIX_EPOCH 116444736000000000ULL
	#define RBC_TICKS_PER_SECOND 10000000U
	#define RBC_NANOSECONDS_PER_TICK 100U

rbc_time rbc_time_now(void) {
	FILETIME ft;
	GetSystemTimePreciseAsFileTime(&ft);
	// QuadPart is # of 100ns ticks since 1601-01-01T00:00:00Z
	u64 const ticks_since_1601 = (ULARGE_INTEGER){.LowPart = ft.dwLowDateTime, .HighPart = ft.dwHighDateTime}.QuadPart;
	u64 const ticks = ticks_since_1601 - RBC_TICKS_FROM_1601_TO_UNIX_EPOCH;
	u64 const secs = ticks / RBC_TICKS_PER_SECOND;
	u64 const nsecs = (ticks - secs * RBC_TICKS_PER_SECOND) * RBC_NANOSECONDS_PER_TICK;
	return (rbc_time){
	    .rep = {(i64) secs, (u32) (nsecs * 4)}
    };
}

	#undef RBC_NANOSECONDS_PER_TICK
	#undef RBC_TICKS_PER_SECOND
	#undef RBC_TICKS_FROM_1601_TO_UNIX_EPOCH

#endif

bool rbc_time_lt(rbc_time lhs, rbc_time rhs) {
	return rbc_duration_lt(lhs.rep, rhs.rep);
}

bool rbc_time_le(rbc_time lhs, rbc_time rhs) {
	return rbc_duration_le(lhs.rep, rhs.rep);
}

bool rbc_time_gt(rbc_time lhs, rbc_time rhs) {
	return rbc_duration_gt(lhs.rep, rhs.rep);
}

bool rbc_time_ge(rbc_time lhs, rbc_time rhs) {
	return rbc_duration_ge(lhs.rep, rhs.rep);
}

bool rbc_time_eq(rbc_time lhs, rbc_time rhs) {
	return rbc_duration_eq(lhs.rep, rhs.rep);
}

bool rbc_time_ne(rbc_time lhs, rbc_time rhs) {
	return rbc_duration_ne(lhs.rep, rhs.rep);
}

rbc_duration rbc_time_sub(rbc_time lhs, rbc_time rhs) {
	return rbc_duration_sub(lhs.rep, rhs.rep);
}

rbc_time rbc_time_add_duration(rbc_time self, rbc_duration d) {
	return (rbc_time){rbc_duration_add(self.rep, d)};
}

rbc_time rbc_time_sub_duration(rbc_time self, rbc_duration d) {
	return (rbc_time){rbc_duration_sub(self.rep, d)};
}

rbc_timespec rbc_time_to_timespec(rbc_time t) {
	return rbc_duration_to_timespec(t.rep);
}

rbc_time rbc_time_from_timespec(rbc_timespec ts) {
	return (rbc_time){.rep = rbc_duration_from_timespec(ts)};
}

rbc_time rbc_time_deadline_from_timeout(rbc_duration timeout) {
	return rbc_time_add_duration(rbc_time_now(), timeout);
}
