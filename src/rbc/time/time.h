#pragma once

#include <rbc/time/duration.h>

struct RbcTime {
	RbcDuration rep;
};

typedef struct RbcTime RbcTime;

#define RBC_TIME_UNIX_EPOCH      \
	(RbcTime) {                  \
		.rep = RBC_DURATION_ZERO \
	}

RBC_BEGIN_EXTERN_C

// factory functions
RBC_EXPORT RbcTime rbc_time_unix_epoch(void);
RBC_EXPORT RbcTime rbc_time_now(void);

// rel. ops
RBC_EXPORT bool rbc_time_lt(RbcTime lhs, RbcTime rhs);
RBC_EXPORT bool rbc_time_le(RbcTime lhs, RbcTime rhs);
RBC_EXPORT bool rbc_time_gt(RbcTime lhs, RbcTime rhs);
RBC_EXPORT bool rbc_time_ge(RbcTime lhs, RbcTime rhs);
RBC_EXPORT bool rbc_time_eq(RbcTime lhs, RbcTime rhs);
RBC_EXPORT bool rbc_time_ne(RbcTime lhs, RbcTime rhs);

// arith. ops
RBC_EXPORT RbcDuration rbc_time_sub(RbcTime lhs, RbcTime rhs);
RBC_EXPORT RbcTime rbc_time_add_duration(RbcTime self, RbcDuration d);
RBC_EXPORT RbcTime rbc_time_sub_duration(RbcTime self, RbcDuration d);

// conversions
RBC_EXPORT RbcTimespec rbc_time_to_timespec(RbcTime t);
RBC_EXPORT RbcTime rbc_time_from_timespec(RbcTimespec ts);

// utils
RBC_EXPORT RbcTime rbc_time_deadline_from_timeout(RbcDuration timeout);

RBC_END_EXTERN_C
