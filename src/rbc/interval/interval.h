#pragma once

#include <rbc/core/attributes.h>
#include <rbc/core/export.h>
#include <rbc/core/extern.h>
#include <rbc/core/types.h>

struct rbc_interval {
	f64 lo;
	f64 hi;
};
typedef struct rbc_interval rbc_interval;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RBC_CONST RBC_NODISCARD RBC_NOTHROW rbc_interval rbc_interval_empty(void);

RBC_EXPORT RBC_CONST RBC_NODISCARD RBC_NOTHROW rbc_interval rbc_interval_new(f64 lo, f64 hi);

static inline RBC_CONST RBC_NODISCARD RBC_NOTHROW rbc_interval rbc_interval_from_value(f64 value) {
	return rbc_interval_new(value, value);
}

RBC_EXPORT RBC_NODISCARD RBC_NOTHROW RBC_PURE rbc_interval rbc_interval_from_center(f64 center, f64 radius);

static inline RBC_CONST RBC_NOTHROW rbc_interval rbc_interval_neg(rbc_interval self) {
	return (rbc_interval) {-self.hi, -self.lo};
}

RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_interval rbc_interval_add(rbc_interval lhs, rbc_interval rhs);

static inline RBC_NOTHROW RBC_PURE rbc_interval rbc_interval_sub(rbc_interval lhs, rbc_interval rhs) {
	return rbc_interval_add(lhs, rbc_interval_neg(rhs));
}

RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_interval rbc_interval_mul(rbc_interval lhs, rbc_interval rhs);

RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_interval_is_empty(rbc_interval self);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_interval_is_nan(rbc_interval self);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_interval_is_whole(rbc_interval self);

RBC_NOTHROW void rbc_interval_print(rbc_interval self);
RBC_NOTHROW void rbc_interval_print_fmt(rbc_interval self, char const* fmt);

RBC_END_EXTERN_C
