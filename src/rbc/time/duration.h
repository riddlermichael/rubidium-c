#pragma once

#include <rbc/core/export.h>
#include <rbc/core/types.h>
#include <rbc/time/timespec.h>

struct rbc_duration {
	i64 secs;
	u32 ticks;
};

typedef struct rbc_duration rbc_duration;

#define RBC_DURATION_ZERO     \
	(rbc_duration) {          \
		.secs = 0, .ticks = 0 \
	}

#define RBC_DURATION_INF                \
	(rbc_duration) {                    \
		.secs = INT64_MAX, .ticks = ~0U \
	}

#define RBC_DURATION_NEG_INF            \
	(rbc_duration) {                    \
		.secs = INT64_MIN, .ticks = ~0U \
	}

// avoid compiler warnings about magic numbers
enum {
	RBC_NANOSECONDS_PER_SECOND = 1000000000,
	RBC_MICROSECONDS_PER_SECOND = 1000000,
	RBC_MILLISECONDS_PER_SECOND = 1000,
	RBC_SECONDS_PER_MINUTE = 60,
	RBC_SECONDS_PER_HOUR = 3600
};

RBC_BEGIN_EXTERN_C

// factory functions
RBC_EXPORT rbc_duration rbc_duration_zero(void);
RBC_EXPORT rbc_duration rbc_duration_inf(void);
RBC_EXPORT rbc_duration rbc_duration_neg_inf(void);
RBC_EXPORT rbc_duration rbc_duration_ns(i64 ns);
RBC_EXPORT rbc_duration rbc_duration_us(i64 us);
RBC_EXPORT rbc_duration rbc_duration_ms(i64 ms);
RBC_EXPORT rbc_duration rbc_duration_s(i64 s);
RBC_EXPORT rbc_duration rbc_duration_min(i64 min);
RBC_EXPORT rbc_duration rbc_duration_h(i64 h);

RBC_EXPORT bool rbc_duration_is_inf(rbc_duration self);
RBC_EXPORT bool rbc_duration_is_neg(rbc_duration self);

// rel. ops
RBC_EXPORT bool rbc_duration_lt(rbc_duration lhs, rbc_duration rhs);
RBC_EXPORT bool rbc_duration_gt(rbc_duration lhs, rbc_duration rhs);
RBC_EXPORT bool rbc_duration_le(rbc_duration lhs, rbc_duration rhs);
RBC_EXPORT bool rbc_duration_ge(rbc_duration lhs, rbc_duration rhs);
RBC_EXPORT bool rbc_duration_eq(rbc_duration lhs, rbc_duration rhs);
RBC_EXPORT bool rbc_duration_ne(rbc_duration lhs, rbc_duration rhs);

// arith. ops
RBC_EXPORT rbc_duration rbc_duration_neg(rbc_duration self);
RBC_EXPORT rbc_duration rbc_duration_add(rbc_duration lhs, rbc_duration rhs);
RBC_EXPORT rbc_duration rbc_duration_sub(rbc_duration lhs, rbc_duration rhs);

// conversions
RBC_EXPORT rbc_timespec rbc_duration_to_timespec(rbc_duration self);
RBC_EXPORT rbc_duration rbc_duration_from_timespec(rbc_timespec ts);

RBC_EXPORT RBC_CONST i64 rbc_duration_to_ns(rbc_duration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_us(rbc_duration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_ms(rbc_duration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_s(rbc_duration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_min(rbc_duration self);
RBC_EXPORT RBC_CONST i64 rbc_duration_to_h(rbc_duration self);

RBC_END_EXTERN_C

#ifdef __cplusplus

	#include <ostream>

inline std::ostream& operator<<(std::ostream& os, rbc_duration dur) {
	return os << "rbc_duration{secs: " << dur.secs << ", ticks: " << dur.ticks << "}";
}

#endif
