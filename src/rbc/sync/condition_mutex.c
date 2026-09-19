#include "condition_mutex.h"

#include <rbc/core/sanitizers.h>
#include <rbc/sync/impl.h>

// RbcConditionMutex

RbcError rbc_condition_mutex_init(RbcConditionMutex* self) {
	RBC_SYNC_CHECK(rbc_mutex_init(&self->mu));
	RBC_SYNC_CHECK(rbc_cond_var_init(&self->cv));
	return RBC_ERROR_OK;
}

RbcError rbc_condition_mutex_destroy(RbcConditionMutex* self) {
	RBC_SYNC_CHECK(rbc_cond_var_destroy(&self->cv));
	return rbc_mutex_destroy(&self->mu);
}

RbcError rbc_condition_mutex_lock(RbcConditionMutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return rbc_mutex_lock(self.mu);
}

RbcError rbc_condition_mutex_lock_for(RbcConditionMutex self, RbcDuration timeout) {
	return rbc_mutex_lock_for(self.mu, timeout);
}

RbcError rbc_condition_mutex_lock_until(RbcConditionMutex self, RbcTime deadline) {
	return rbc_mutex_lock_until(self.mu, deadline);
}

RbcError rbc_condition_mutex_try_lock(RbcConditionMutex self) {
	return rbc_mutex_try_lock(self.mu);
}

RbcError rbc_condition_mutex_unlock(RbcConditionMutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
	return rbc_mutex_unlock(self.mu);
}

RbcError rbc_condition_mutex_await(RbcConditionMutex self, RbcCondition condition) RBC_NO_THREAD_SAFETY_ANALYSIS {
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

RbcError rbc_condition_mutex_await_for(RbcConditionMutex self, RbcCondition condition, RbcDuration timeout) {
	return rbc_condition_mutex_await_until(self, condition, rbc_time_deadline_from_timeout(timeout));
}

RbcError rbc_condition_mutex_await_until(
    RbcConditionMutex self, RbcCondition condition, RbcTime deadline) RBC_NO_THREAD_SAFETY_ANALYSIS {
	if (rbc_condition_eval(condition)) {
		return RBC_ERROR_OK;
	}

	RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
	while (true) {
		RBC_SYNC_CHECK(rbc_cond_var_notify_all(self.cv));
		RbcError const error = rbc_cond_var_wait_until(self.cv, self.mu, deadline);
		if (error == RBC_ERROR_TIMED_OUT) {
			return RBC_ERROR_TIMED_OUT;
		}

		if (rbc_condition_eval(condition)) {
			return RBC_ERROR_OK;
		}
	}
}

RbcError rbc_condition_mutex_lock_when(RbcConditionMutex self, RbcCondition condition) RBC_NO_THREAD_SAFETY_ANALYSIS {
	RBC_SYNC_CHECK(rbc_condition_mutex_lock(self));
	return rbc_condition_mutex_await(self, condition);
}

RbcError rbc_condition_mutex_lock_when_for(RbcConditionMutex self, RbcCondition condition, RbcDuration timeout) {
	return rbc_condition_mutex_lock_when_until(self, condition, rbc_time_deadline_from_timeout(timeout));
}

RbcError rbc_condition_mutex_lock_when_until(
    RbcConditionMutex self, RbcCondition condition, RbcTime deadline) RBC_NO_THREAD_SAFETY_ANALYSIS {
	RBC_SYNC_CHECK(rbc_condition_mutex_lock(self));
	return rbc_condition_mutex_await_until(self, condition, deadline);
}

// RbcCondition

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
	return *(bool*) arg;
}

RbcCondition rbc_condition_from_fn(RbcConditionFn fn, void* arg) {
	return (RbcCondition) {.fn = fn, .arg = arg};
}

RbcCondition rbc_condition_from_bool(bool const* cond) {
	return (RbcCondition) {.fn = dereference, .arg = (void*) cond};
}

bool rbc_condition_eval(RbcCondition self) {
	return (*self.fn)(self.arg);
}
