#pragma once

#include <rbc/time/duration.h>

struct rbc_time {
	rbc_duration rep;
};

typedef struct rbc_time rbc_time;

#define RBC_TIME_UNIX_EPOCH      \
	(rbc_time) {                 \
		.rep = RBC_DURATION_ZERO \
	}

RBC_BEGIN_EXTERN_C

// factory functions
RBC_EXPORT rbc_time rbc_time_unix_epoch(void);
RBC_EXPORT rbc_time rbc_time_now(void);

// rel. ops
RBC_EXPORT bool rbc_time_lt(rbc_time lhs, rbc_time rhs);
RBC_EXPORT bool rbc_time_le(rbc_time lhs, rbc_time rhs);
RBC_EXPORT bool rbc_time_gt(rbc_time lhs, rbc_time rhs);
RBC_EXPORT bool rbc_time_ge(rbc_time lhs, rbc_time rhs);
RBC_EXPORT bool rbc_time_eq(rbc_time lhs, rbc_time rhs);
RBC_EXPORT bool rbc_time_ne(rbc_time lhs, rbc_time rhs);

// arith. ops
RBC_EXPORT rbc_duration rbc_time_sub(rbc_time lhs, rbc_time rhs);
RBC_EXPORT rbc_time rbc_time_add_duration(rbc_time self, rbc_duration d);
RBC_EXPORT rbc_time rbc_time_sub_duration(rbc_time self, rbc_duration d);

// conversions
RBC_EXPORT rbc_timespec rbc_time_to_timespec(rbc_time t);
RBC_EXPORT rbc_time rbc_time_from_timespec(rbc_timespec ts);

// utils
RBC_EXPORT rbc_time rbc_time_deadline_from_timeout(rbc_duration timeout);

RBC_END_EXTERN_C
