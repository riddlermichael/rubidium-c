#include "duration.h"

#include <rbc/core/builtins.h>
#include <rbc/core/limits.h>

static RBC_CONST RBC_NOTHROW RbcDuration make_duration(i64 secs, i64 ticks) {
	return (RbcDuration) {.secs = secs, .ticks = (u32) ticks};
}

static RBC_CONST RBC_NOTHROW RbcDuration make_normalized_duration(i64 secs, i64 ticks) {
	return ticks < 0
	    ? make_duration(secs - 1, ticks + RBC_TICKS_PER_SECOND)
	    : make_duration(secs, ticks);
}

static RBC_CONST RBC_NOTHROW RbcDuration from_i64(i64 value, i64 ratio) {
	i64 const rem = value % ratio;
	return make_normalized_duration(value / ratio, (rem * (i64) RBC_TICKS_PER_SECOND) / ratio);
}

RbcDuration rbc_duration_zero(void) {
	return RBC_DURATION_ZERO;
}

RbcDuration rbc_duration_inf(void) {
	return RBC_DURATION_INF;
}

RbcDuration rbc_duration_neg_inf(void) {
	return RBC_DURATION_NEG_INF;
}

RbcDuration rbc_duration_ns(i64 ns) {
	return from_i64(ns, RBC_NANOSECONDS_PER_SECOND);
}

RbcDuration rbc_duration_us(i64 us) {
	return from_i64(us, RBC_MICROSECONDS_PER_SECOND);
}

RbcDuration rbc_duration_ms(i64 ms) {
	return from_i64(ms, RBC_MILLISECONDS_PER_SECOND);
}

RbcDuration rbc_duration_s(i64 s) {
	return from_i64(s, 1);
}

RbcDuration rbc_duration_min(i64 min) {
	return min <= RBC_I64_MAX / RBC_SECONDS_PER_MINUTE && min >= RBC_I64_MIN / RBC_SECONDS_PER_MINUTE
	    ? (RbcDuration) {.secs = min * RBC_SECONDS_PER_MINUTE, .ticks = 0}
	    : min > 0 ? RBC_DURATION_INF
	              : RBC_DURATION_NEG_INF;
}

RbcDuration rbc_duration_h(i64 h) {
	return h <= RBC_I64_MAX / RBC_SECONDS_PER_HOUR && h >= RBC_I64_MIN / RBC_SECONDS_PER_HOUR
	    ? (RbcDuration) {.secs = h * RBC_SECONDS_PER_HOUR, .ticks = 0}
	    : h > 0 ? RBC_DURATION_INF
	            : RBC_DURATION_NEG_INF;
}

bool rbc_duration_is_inf(RbcDuration self) {
	return self.ticks == RBC_INF_TICKS;
}

bool rbc_duration_is_neg(RbcDuration self) {
	return self.secs < 0;
}

bool rbc_duration_lt(RbcDuration lhs, RbcDuration rhs) {
	if (lhs.secs != rhs.secs) {
		return lhs.secs < rhs.secs;
	}

	if (lhs.secs == RBC_I64_MIN) {
		return lhs.ticks + 1 < rhs.ticks + 1;
	}

	return lhs.ticks < rhs.ticks;
}

bool rbc_duration_gt(RbcDuration lhs, RbcDuration rhs) {
	return rbc_duration_lt(rhs, lhs); // NOLINT(readability-suspicious-call-argument)
}

bool rbc_duration_le(RbcDuration lhs, RbcDuration rhs) {
	return !rbc_duration_lt(rhs, lhs); // NOLINT(readability-suspicious-call-argument)
}

bool rbc_duration_ge(RbcDuration lhs, RbcDuration rhs) {
	return !rbc_duration_lt(lhs, rhs);
}

bool rbc_duration_eq(RbcDuration lhs, RbcDuration rhs) {
	return lhs.secs == rhs.secs && lhs.ticks == rhs.ticks;
}

bool rbc_duration_ne(RbcDuration lhs, RbcDuration rhs) {
	return !rbc_duration_eq(lhs, rhs);
}

RbcDuration rbc_duration_neg(RbcDuration self) {
	if (self.ticks == 0) {
		return self.secs == RBC_I64_MIN
		    ? RBC_DURATION_INF
		    : (RbcDuration) {.secs = -self.secs, .ticks = 0};
	}

	if (rbc_duration_is_inf(self)) {
		return self.secs < 0 ? RBC_DURATION_INF : RBC_DURATION_NEG_INF;
	}

	return (RbcDuration) {.secs = ~self.secs, .ticks = RBC_TICKS_PER_SECOND - self.ticks};
}

RbcDuration rbc_duration_add(RbcDuration lhs, RbcDuration rhs) {
	if (rbc_duration_is_inf(lhs)) {
		return lhs;
	}

	if (rbc_duration_is_inf(rhs)) {
		return rhs;
	}

	i64 secs = lhs.secs + rhs.secs;
	u32 ticks = lhs.ticks;
	if (ticks >= RBC_TICKS_PER_SECOND - rhs.ticks) {
		++secs;
		ticks -= RBC_TICKS_PER_SECOND;
	}
	ticks += rhs.ticks;
	if (rhs.secs < 0 ? secs > lhs.secs : secs < lhs.secs) {
		return rhs.secs < 0 ? RBC_DURATION_NEG_INF : RBC_DURATION_INF;
	}

	return (RbcDuration) {.secs = secs, .ticks = ticks};
}

RbcDuration rbc_duration_sub(RbcDuration lhs, RbcDuration rhs) {
	if (rbc_duration_is_inf(lhs)) {
		return lhs;
	}

	if (rbc_duration_is_inf(rhs)) {
		return rhs.secs > 0 ? RBC_DURATION_NEG_INF : RBC_DURATION_INF;
	}

	i64 secs = lhs.secs - rhs.secs;
	u32 ticks = lhs.ticks;
	if (ticks < rhs.ticks) {
		--secs;
		ticks += RBC_TICKS_PER_SECOND;
	}
	ticks -= rhs.ticks;
	if (rhs.secs < 0 ? secs < lhs.secs : secs > lhs.secs) {
		return rhs.secs >= 0 ? RBC_DURATION_NEG_INF : RBC_DURATION_INF;
	}

	return (RbcDuration) {.secs = secs, .ticks = ticks};
}

RbcTimespec rbc_duration_to_timespec(RbcDuration self) {
	RbcTimespec ts;
	if (!rbc_duration_is_inf(self)) {
		i64 secs = self.secs;
		u32 ticks = self.ticks;
		if (secs < 0) {
			// tweak the fields so that unsigned division of `ticks` maps to truncation (towards zero) for the timespec
			ticks += RBC_TICKS_PER_NANOSECOND - 1;
			if (ticks >= RBC_TICKS_PER_SECOND) {
				++secs;
				ticks -= RBC_TICKS_PER_SECOND;
			}
		}
		ts.tv_sec = (time_t) secs;
		if (ts.tv_sec == secs) { // no time_t narrowing
			ts.tv_nsec = (long) (ticks / RBC_TICKS_PER_NANOSECOND);
			return ts;
		}
	}

	// such time_t values are invalid for std-library functions
	if (rbc_duration_is_neg(self)) {
		ts.tv_sec = RBC_MIN(time_t);
		ts.tv_nsec = 0;
	} else {
		ts.tv_sec = RBC_MAX(time_t);
		ts.tv_nsec = RBC_NANOSECONDS_PER_SECOND - 1;
	}
	return ts;
}

RbcDuration rbc_duration_from_timespec(RbcTimespec ts) {
	if (RBC_LIKELY(ts.tv_nsec < RBC_NANOSECONDS_PER_SECOND)) {
		return (RbcDuration) {.secs = ts.tv_sec, .ticks = ts.tv_nsec * RBC_TICKS_PER_NANOSECOND};
	}

	RbcDuration const s = rbc_duration_s(ts.tv_sec);
	RbcDuration const ns = rbc_duration_ns(ts.tv_nsec);
	return rbc_duration_add(s, ns);
}

RbcDuration rbc_duration_from_std_timespec(StdTimespec ts) {
	return rbc_duration_from_timespec(rbc_timespec_from_std_timespec(ts));
}

i64 rbc_duration_to_ns(RbcDuration self) {
	if (RBC_UNLIKELY(rbc_duration_is_inf(self))) {
		return self.secs;
	}

	return self.secs * RBC_NANOSECONDS_PER_SECOND
	    + self.ticks / RBC_TICKS_PER_NANOSECOND;
}

i64 rbc_duration_to_us(RbcDuration self) {
	if (RBC_UNLIKELY(rbc_duration_is_inf(self))) {
		return self.secs;
	}

	return self.secs * RBC_MICROSECONDS_PER_SECOND
	    + self.ticks / (RBC_TICKS_PER_NANOSECOND * 1000);
}

i64 rbc_duration_to_ms(RbcDuration self) {
	if (RBC_UNLIKELY(rbc_duration_is_inf(self))) {
		return self.secs;
	}

	return self.secs * RBC_MILLISECONDS_PER_SECOND
	    + self.ticks / (RBC_TICKS_PER_NANOSECOND * 1000 * 1000);
}

i64 rbc_duration_to_s(RbcDuration self) {
	if (RBC_UNLIKELY(rbc_duration_is_inf(self))) {
		return self.secs;
	}

	return self.secs + (self.secs < 0 && self.ticks);
}

i64 rbc_duration_to_min(RbcDuration self) {
	if (RBC_UNLIKELY(rbc_duration_is_inf(self))) {
		return self.secs;
	}

	return rbc_duration_to_s(self) / RBC_SECONDS_PER_MINUTE;
}

i64 rbc_duration_to_h(RbcDuration self) {
	if (RBC_UNLIKELY(rbc_duration_is_inf(self))) {
		return self.secs;
	}

	return rbc_duration_to_s(self) / RBC_SECONDS_PER_HOUR;
}
