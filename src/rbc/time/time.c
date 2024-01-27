#include "time.h"

rbc_time rbc_time_unix_epoch(void) {
	return RBC_TIME_UNIX_EPOCH;
}

rbc_time rbc_time_now(void) {
	rbc_timespec const ts = rbc_timespec_get();
	return (rbc_time){rbc_duration_from_timespec(ts)};
}

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
