#pragma once

#include <rbc/core/export.h>
#include <rbc/core/types.h>
#include <rbc/time/timespec.h>

struct RbcDuration {
	i64 secs;
	u32 ticks;
};

typedef struct RbcDuration RbcDuration;

#define RBC_DURATION_ZERO (RbcDuration){.secs = 0, .ticks = 0}

#define RBC_INF_TICKS (~(u32) 0U)

#define RBC_DURATION_INF                          \
	(RbcDuration) {                               \
		.secs = INT64_MAX, .ticks = RBC_INF_TICKS \
	}

#define RBC_DURATION_NEG_INF                      \
	(RbcDuration) {                               \
		.secs = INT64_MIN, .ticks = RBC_INF_TICKS \
	}

#define RBC_TICKS_PER_SECOND ((u32) 4000000000U)
#define RBC_TICKS_PER_NANOSECOND 4

enum {
	RBC_MICROSECONDS_PER_SECOND = 1000000,
	RBC_MILLISECONDS_PER_SECOND = 1000,
	RBC_SECONDS_PER_MINUTE = 60,
	RBC_SECONDS_PER_HOUR = 3600,
};

RBC_BEGIN_EXTERN_C

// factory functions
RBC_EXPORT RbcDuration rbc_duration_zero(void);
RBC_EXPORT RbcDuration rbc_duration_inf(void);
RBC_EXPORT RbcDuration rbc_duration_neg_inf(void);
RBC_EXPORT RbcDuration rbc_duration_ns(i64 ns);
RBC_EXPORT RbcDuration rbc_duration_us(i64 us);
RBC_EXPORT RbcDuration rbc_duration_ms(i64 ms);
RBC_EXPORT RbcDuration rbc_duration_s(i64 s);
RBC_EXPORT RbcDuration rbc_duration_min(i64 min);
RBC_EXPORT RbcDuration rbc_duration_h(i64 h);

RBC_EXPORT bool rbc_duration_is_inf(RbcDuration self);
RBC_EXPORT bool rbc_duration_is_neg(RbcDuration self);

// rel. ops
RBC_EXPORT bool rbc_duration_lt(RbcDuration lhs, RbcDuration rhs);
RBC_EXPORT bool rbc_duration_gt(RbcDuration lhs, RbcDuration rhs);
RBC_EXPORT bool rbc_duration_le(RbcDuration lhs, RbcDuration rhs);
RBC_EXPORT bool rbc_duration_ge(RbcDuration lhs, RbcDuration rhs);
RBC_EXPORT bool rbc_duration_eq(RbcDuration lhs, RbcDuration rhs);
RBC_EXPORT bool rbc_duration_ne(RbcDuration lhs, RbcDuration rhs);

// arith. ops
RBC_EXPORT RbcDuration rbc_duration_neg(RbcDuration self);
RBC_EXPORT RbcDuration rbc_duration_add(RbcDuration lhs, RbcDuration rhs);
RBC_EXPORT RbcDuration rbc_duration_sub(RbcDuration lhs, RbcDuration rhs);

// conversions
RBC_EXPORT RbcTimespec rbc_duration_to_timespec(RbcDuration self);
RBC_EXPORT RbcDuration rbc_duration_from_timespec(RbcTimespec ts);
RBC_EXPORT RbcDuration rbc_duration_from_std_timespec(StdTimespec ts);

RBC_EXPORT RBC_CONST i64 rbc_duration_to_ns(RbcDuration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_us(RbcDuration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_ms(RbcDuration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_s(RbcDuration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_min(RbcDuration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_h(RbcDuration self);

RBC_END_EXTERN_C

#ifdef __cplusplus

	#include <ostream>

inline std::ostream& operator<<(std::ostream& os, RbcDuration dur) {
	return os << "RbcDuration{secs: " << dur.secs << ", ticks: " << dur.ticks << "}";
}

#endif
