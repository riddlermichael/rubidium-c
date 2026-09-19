#include "time.h"

RbcTime rbc_time_unix_epoch(void) {
	return RBC_TIME_UNIX_EPOCH;
}

RbcTime rbc_time_now(void) {
	RbcTimespec const ts = rbc_timespec_get();
	return (RbcTime) {rbc_duration_from_timespec(ts)};
}

bool rbc_time_lt(RbcTime lhs, RbcTime rhs) {
	return rbc_duration_lt(lhs.rep, rhs.rep);
}

bool rbc_time_le(RbcTime lhs, RbcTime rhs) {
	return rbc_duration_le(lhs.rep, rhs.rep);
}

bool rbc_time_gt(RbcTime lhs, RbcTime rhs) {
	return rbc_duration_gt(lhs.rep, rhs.rep);
}

bool rbc_time_ge(RbcTime lhs, RbcTime rhs) {
	return rbc_duration_ge(lhs.rep, rhs.rep);
}

bool rbc_time_eq(RbcTime lhs, RbcTime rhs) {
	return rbc_duration_eq(lhs.rep, rhs.rep);
}

bool rbc_time_ne(RbcTime lhs, RbcTime rhs) {
	return rbc_duration_ne(lhs.rep, rhs.rep);
}

RbcDuration rbc_time_sub(RbcTime lhs, RbcTime rhs) {
	return rbc_duration_sub(lhs.rep, rhs.rep);
}

RbcTime rbc_time_add_duration(RbcTime self, RbcDuration d) {
	return (RbcTime) {rbc_duration_add(self.rep, d)};
}

RbcTime rbc_time_sub_duration(RbcTime self, RbcDuration d) {
	return (RbcTime) {rbc_duration_sub(self.rep, d)};
}

RbcTimespec rbc_time_to_timespec(RbcTime t) {
	return rbc_duration_to_timespec(t.rep);
}

RbcTime rbc_time_from_timespec(RbcTimespec ts) {
	return (RbcTime) {rbc_duration_from_timespec(ts)};
}

RbcTime rbc_time_deadline_from_timeout(RbcDuration timeout) {
	return rbc_time_add_duration(rbc_time_now(), timeout);
}
