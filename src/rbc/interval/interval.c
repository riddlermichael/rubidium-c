#include "interval.h"

#include <fenv.h>
#include <float.h>
#include <math.h>

#include <rbc/core/assert.h>
#include <rbc/core/limits.h>

RBC_STATIC_ASSERT(FLT_RADIX == 2);

RBC_STATIC_ASSERT(sizeof(float) == 4);
RBC_STATIC_ASSERT(FLT_MANT_DIG == 24);
RBC_STATIC_ASSERT(FLT_MAX_EXP == 128);
RBC_STATIC_ASSERT(FLT_MIN_EXP == -125);

RBC_STATIC_ASSERT(sizeof(double) == 8);
RBC_STATIC_ASSERT(DBL_MANT_DIG == 53);
RBC_STATIC_ASSERT(DBL_MAX_EXP == 1024);
RBC_STATIC_ASSERT(DBL_MIN_EXP == -1021);

#pragma STDC FENV_ACCESS ON

rbc_interval rbc_interval_empty(void) {
}

rbc_interval rbc_interval_new(f64 lo, f64 hi) {
	RBC_ASSERT(lo <= hi); // TODO
	return (rbc_interval) {lo, hi};
}

rbc_interval rbc_interval_from_center(f64 center, f64 radius) {
	int const round = fegetround();

	fesetround(FE_DOWNWARD);
	f64 const lo = center - radius;

	fesetround(FE_UPWARD);
	f64 const hi = center + radius;

	fesetround(round);
	return (rbc_interval) {lo, hi};
}

rbc_interval rbc_interval_add(rbc_interval lhs, rbc_interval rhs) {
	int const round = fegetround();

	fesetround(FE_DOWNWARD);
	f64 const lo = lhs.lo + rhs.lo;

	fesetround(FE_UPWARD);
	f64 const hi = lhs.hi + rhs.hi;

	fesetround(round);
	return (rbc_interval) {lo, hi};
}

static RBC_NOTHROW RBC_PURE f64 min(f64 const* values, usize count) {
	f64 result = RBC_F64_INF;
	for (usize pos = 0; pos < count; ++pos) {
		if (result > values[pos]) {
			result = values[pos];
		}
	}
	return result;
}

static RBC_NOTHROW RBC_PURE f64 max(f64 const* values, usize count) {
	f64 result = -RBC_F64_INF;
	for (usize pos = 0; pos < count; ++pos) {
		if (result < values[pos]) {
			result = values[pos];
		}
	}
	return result;
}

rbc_interval rbc_interval_mul(rbc_interval lhs, rbc_interval rhs) {
	int const round = fegetround();

	fesetround(FE_DOWNWARD);
	f64 const down_products[] = {
	    lhs.lo * rhs.lo,
	    lhs.lo * rhs.hi,
	    lhs.hi * rhs.lo,
	    lhs.hi * rhs.hi,
	};
	f64 const lo = min(down_products, 4);

	fesetround(FE_UPWARD);
	f64 const up_products[] = {
	    lhs.lo * rhs.lo,
	    lhs.lo * rhs.hi,
	    lhs.hi * rhs.lo,
	    lhs.hi * rhs.hi,
	};
	f64 const hi = max(up_products, 4);

	fesetround(round);
	return (rbc_interval) {lo, hi};
}

bool rbc_interval_is_empty(rbc_interval self) {
	return isnan(self.lo) || isnan(self.hi); // FIXME
}

bool rbc_interval_is_nan(rbc_interval self) {
	return isnan(self.lo) || isnan(self.hi); // FIXME
}

bool rbc_interval_is_whole(rbc_interval self) {
	return self.lo == -RBC_F64_INF
	    && self.hi == RBC_F64_INF;
}

void rbc_interval_print(rbc_interval self) {
	rbc_interval_print_fmt(self, "%.20lf");
}

void rbc_interval_print_fmt(rbc_interval self, char const* fmt) {
	printf("[");
	printf(fmt, self.lo);
	printf(", ");
	printf(fmt, self.hi);
	printf("]\n");
}
