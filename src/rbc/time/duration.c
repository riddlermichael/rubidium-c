#include "duration.h"

#include <rbc/core/builtins.h>
#include <rbc/core/limits.h>

static rbc_duration make_duration(i64 secs, i64 ticks) {
	return (rbc_duration) {.secs = secs, .ticks = (u32) ticks};
}

static rbc_duration make_normalized_duration(i64 secs, i64 ticks) {
	return (ticks < 0)
	    ? make_duration(secs - 1, ticks + RBC_TICKS_PER_SECOND)
	    : make_duration(secs, ticks);
}

static rbc_duration from_i64(i64 value, i64 ratio) {
	return make_normalized_duration(value / ratio, value % ratio * RBC_TICKS_PER_SECOND / ratio);
}

rbc_duration rbc_duration_zero(void) {
	return RBC_DURATION_ZERO;
}

rbc_duration rbc_duration_inf(void) {
	return RBC_DURATION_INF;
}

rbc_duration rbc_duration_neg_inf(void) {
	return RBC_DURATION_NEG_INF;
}

rbc_duration rbc_duration_ns(i64 ns) {
	return from_i64(ns, RBC_NANOSECONDS_PER_SECOND);
}

rbc_duration rbc_duration_us(i64 us) {
	return from_i64(us, RBC_MICROSECONDS_PER_SECOND);
}

rbc_duration rbc_duration_ms(i64 ms) {
	return from_i64(ms, RBC_MILLISECONDS_PER_SECOND);
}

rbc_duration rbc_duration_s(i64 s) {
	return from_i64(s, 1);
}

rbc_duration rbc_duration_min(i64 min) {
	return min <= RBC_I64_MAX / RBC_SECONDS_PER_MINUTE && min >= RBC_I64_MIN / RBC_SECONDS_PER_MINUTE
	    ? (rbc_duration) {min * RBC_SECONDS_PER_MINUTE, 0}
	    : min > 0 ? RBC_DURATION_INF
	              : RBC_DURATION_NEG_INF;
}

rbc_duration rbc_duration_h(i64 h) {
	return h <= RBC_I64_MAX / RBC_SECONDS_PER_HOUR && h >= RBC_I64_MIN / RBC_SECONDS_PER_HOUR
	    ? (rbc_duration) {h * RBC_SECONDS_PER_HOUR, 0}
	    : h > 0 ? RBC_DURATION_INF
	            : RBC_DURATION_NEG_INF;
}

bool rbc_duration_is_inf(rbc_duration self) {
	return self.ticks == RBC_INF_TICKS;
}

bool rbc_duration_is_neg(rbc_duration self) {
	return self.secs < 0;
}

bool rbc_duration_lt(rbc_duration lhs, rbc_duration rhs) {
	if (lhs.secs != rhs.secs) {
		return lhs.secs < rhs.secs;
	}

	if (lhs.secs == RBC_I64_MIN) {
		return lhs.ticks + 1 < rhs.ticks + 1;
	}

	return lhs.ticks < rhs.ticks;
}

bool rbc_duration_gt(rbc_duration lhs, rbc_duration rhs) {
	return rbc_duration_lt(rhs, lhs); // NOLINT(readability-suspicious-call-argument)
}

bool rbc_duration_le(rbc_duration lhs, rbc_duration rhs) {
	return !rbc_duration_lt(rhs, lhs); // NOLINT(readability-suspicious-call-argument)
}

bool rbc_duration_ge(rbc_duration lhs, rbc_duration rhs) {
	return !rbc_duration_lt(lhs, rhs);
}

bool rbc_duration_eq(rbc_duration lhs, rbc_duration rhs) {
	return lhs.secs == rhs.secs && lhs.ticks == rhs.ticks;
}

bool rbc_duration_ne(rbc_duration lhs, rbc_duration rhs) {
	return !rbc_duration_eq(lhs, rhs);
}

rbc_duration rbc_duration_neg(rbc_duration self) {
	if (self.ticks == 0) {
		return self.secs == RBC_I64_MIN
		    ? RBC_DURATION_INF
		    : (rbc_duration) {-self.secs, 0};
	}

	if (rbc_duration_is_inf(self)) {
		return self.secs < 0 ? RBC_DURATION_INF : RBC_DURATION_NEG_INF;
	}

	return (rbc_duration) {~self.secs, RBC_TICKS_PER_SECOND - self.ticks};
}

rbc_duration rbc_duration_add(rbc_duration lhs, rbc_duration rhs) {
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

	return (rbc_duration) {.secs = secs, .ticks = ticks};
}

rbc_duration rbc_duration_sub(rbc_duration lhs, rbc_duration rhs) {
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

	return (rbc_duration) {.secs = secs, .ticks = ticks};
}

rbc_timespec rbc_duration_to_timespec(rbc_duration self) {
	rbc_timespec ts;
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

rbc_duration rbc_duration_from_timespec(rbc_timespec ts) {
	if (RBC_LIKELY(ts.tv_nsec < RBC_NANOSECONDS_PER_SECOND)) {
		return (rbc_duration) {ts.tv_sec, ts.tv_nsec * RBC_TICKS_PER_NANOSECOND};
	}

	rbc_duration const s = rbc_duration_s(ts.tv_sec);
	rbc_duration const ns = rbc_duration_ns(ts.tv_nsec);
	return rbc_duration_add(s, ns);
}

rbc_duration rbc_duration_from_std_timespec(std_timespec ts) {
	return rbc_duration_from_timespec(rbc_timespec_from_std_timespec(ts));
}

i64 rbc_duration_to_ns(rbc_duration self) {
	return self.secs * RBC_NANOSECONDS_PER_SECOND
	    + self.ticks / RBC_TICKS_PER_NANOSECOND;
}

i64 rbc_duration_to_us(rbc_duration self) {
	return self.secs * RBC_MICROSECONDS_PER_SECOND
	    + self.ticks / (RBC_TICKS_PER_NANOSECOND * 1000);
}

i64 rbc_duration_to_ms(rbc_duration self) {
	return self.secs * RBC_MILLISECONDS_PER_SECOND
	    + self.ticks / (RBC_TICKS_PER_NANOSECOND * 1000 * 1000);
}

i64 rbc_duration_to_s(rbc_duration self) {
	if (rbc_duration_is_inf(self)) {
		return self.secs;
	}

	return self.secs + (self.secs < 0 && self.ticks);
}

i64 rbc_duration_to_min(rbc_duration self) {
	return rbc_duration_to_s(self) / RBC_SECONDS_PER_MINUTE;
}

i64 rbc_duration_to_h(rbc_duration self) {
	return rbc_duration_to_s(self) / RBC_SECONDS_PER_HOUR;
}
