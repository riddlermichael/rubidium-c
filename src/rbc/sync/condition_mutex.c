#include "condition_mutex.h"

#include <rbc/core/sanitizers.h>
#include <rbc/sync/impl.h>

// rbc_condition_mutex

rbc_error rbc_condition_mutex_init(rbc_condition_mutex* self) {
	RBC_SYNC_CHECK(rbc_mutex_init(&self->mu));
	RBC_SYNC_CHECK(rbc_cond_var_init(&self->cv));
	return RBC_ERROR_OK;
}

rbc_error rbc_condition_mutex_destroy(rbc_condition_mutex* self) {
	RBC_SYNC_CHECK(rbc_cond_var_destroy(&self->cv));
	return rbc_mutex_destroy(&self->mu);
}

rbc_error rbc_condition_mutex_lock(rbc_condition_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return rbc_mutex_lock(self.mu);
}

rbc_error rbc_condition_mutex_lock_for(rbc_condition_mutex self, rbc_duration timeout) {
	return rbc_mutex_lock_for(self.mu, timeout);
}

rbc_error rbc_condition_mutex_lock_until(rbc_condition_mutex self, rbc_time deadline) {
	return rbc_mutex_lock_until(self.mu, deadline);
}

rbc_error rbc_condition_mutex_try_lock(rbc_condition_mutex self) {
	return rbc_mutex_try_lock(self.mu);
}

rbc_error rbc_condition_mutex_unlock(rbc_condition_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
	return rbc_mutex_unlock(self.mu);
}

rbc_error rbc_condition_mutex_await(rbc_condition_mutex self, rbc_condition condition) RBC_NO_THREAD_SAFETY_ANALYSIS {
	if (rbc_condition_eval(condition)) {
		return RBC_ERROR_OK;
	}

	RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
	do {
		RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
		RBC_SYNC_CHECK(rbc_cond_var_wait(self.cv, self.mu));
	} while (!rbc_condition_eval(condition));
	return RBC_ERROR_OK;
}

rbc_error rbc_condition_mutex_await_for(rbc_condition_mutex self, rbc_condition condition, rbc_duration timeout) {
	return rbc_condition_mutex_await_until(self, condition, rbc_time_deadline_from_timeout(timeout));
}

rbc_error rbc_condition_mutex_await_until(
    rbc_condition_mutex self, rbc_condition condition, rbc_time deadline) RBC_NO_THREAD_SAFETY_ANALYSIS {
	if (rbc_condition_eval(condition)) {
		return RBC_ERROR_OK;
	}

	RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
	while (true) {
		RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
		rbc_error const error = rbc_cond_var_wait_until(self.cv, self.mu, deadline);
		if (error == RBC_ERROR_TIMED_OUT) {
			return RBC_ERROR_TIMED_OUT;
		}

		if (rbc_condition_eval(condition)) {
			return RBC_ERROR_OK;
		}
	}
}

rbc_error rbc_condition_mutex_lock_when(rbc_condition_mutex self, rbc_condition condition) RBC_NO_THREAD_SAFETY_ANALYSIS {
	RBC_SYNC_CHECK(rbc_condition_mutex_lock(self));
	return rbc_condition_mutex_await(self, condition);
}

rbc_error rbc_condition_mutex_lock_when_for(rbc_condition_mutex self, rbc_condition condition, rbc_duration timeout) {
	return rbc_condition_mutex_lock_when_until(self, condition, rbc_time_deadline_from_timeout(timeout));
}

rbc_error rbc_condition_mutex_lock_when_until(
    rbc_condition_mutex self, rbc_condition condition, rbc_time deadline) RBC_NO_THREAD_SAFETY_ANALYSIS {
	RBC_SYNC_CHECK(rbc_condition_mutex_lock(self));
	return rbc_condition_mutex_await_until(self, condition, deadline);
}

// rbc_condition

#if RBC_ENABLED(THREAD_SANITIZER)
void __tsan_read1(void* addr); // NOLINT
#else
// NOLINTNEXTLINE
	#define __tsan_read1(addr)
#endif

// A function that just returns its argument, dereferenced
static bool dereference(void* arg) {
	// ThreadSanitizer does not instrument this file for memory accesses.
	// This function dereferences a user variable that can participate in a data race,
	// so we need to manually tell TSan about this memory access.
	__tsan_read1(arg);
	return *(bool*) (arg);
}

rbc_condition rbc_condition_from_fn(rbc_condition_fn fn, void* arg) {
	return (rbc_condition){fn, arg};
}

rbc_condition rbc_condition_from_bool(bool const* cond) {
	return (rbc_condition){dereference, (void*) cond};
}

bool rbc_condition_eval(rbc_condition self) {
	return (*self.fn)(self.arg);
}
