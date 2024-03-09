#pragma once

#include <rbc/sync/cond_var.h>

struct RBC_CAPABILITY("rbc_condition_mutex") rbc_condition_mutex {
	rbc_mutex mu;
	rbc_cond_var cv;
};
typedef struct rbc_condition_mutex rbc_condition_mutex;

RBC_BEGIN_EXTERN_C
typedef bool (*rbc_condition_fn)(void*);
RBC_END_EXTERN_C

struct rbc_condition {
	rbc_condition_fn fn;
	void* arg;
};
typedef struct rbc_condition rbc_condition;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_condition_mutex_init(rbc_condition_mutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT rbc_error rbc_condition_mutex_destroy(rbc_condition_mutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT rbc_error rbc_condition_mutex_lock(rbc_condition_mutex self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT rbc_error rbc_condition_mutex_lock_for(rbc_condition_mutex self, rbc_duration timeout)
    RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT rbc_error rbc_condition_mutex_lock_until(rbc_condition_mutex self, rbc_time deadline)
    RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT rbc_error rbc_condition_mutex_try_lock(rbc_condition_mutex self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT rbc_error rbc_condition_mutex_unlock(rbc_condition_mutex self) RBC_RELEASE_CAPABILITY(self);

RBC_EXPORT rbc_error rbc_condition_mutex_await(rbc_condition_mutex self, rbc_condition condition);
RBC_EXPORT rbc_error rbc_condition_mutex_await_for(rbc_condition_mutex self, rbc_condition condition, rbc_duration timeout);
RBC_EXPORT rbc_error rbc_condition_mutex_await_until(rbc_condition_mutex self, rbc_condition condition, rbc_time deadline);
RBC_EXPORT rbc_error rbc_condition_mutex_lock_when(rbc_condition_mutex self, rbc_condition condition) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT rbc_error rbc_condition_mutex_lock_when_for(rbc_condition_mutex self, rbc_condition condition, rbc_duration timeout);
RBC_EXPORT rbc_error rbc_condition_mutex_lock_when_until(rbc_condition_mutex self, rbc_condition condition, rbc_time deadline);

/// Make rbc_condition that returns the result of `(*fn)(arg)`.
/// Equivalent of `(rbc_condition){fn, arg}`.
RBC_EXPORT rbc_condition rbc_condition_from_fn(rbc_condition_fn fn, void* arg);

/// Make rbc_condition that returns the value of `*cond`.
RBC_EXPORT rbc_condition rbc_condition_from_bool(bool const* cond);

/// Evaluates the condition.
RBC_EXPORT bool rbc_condition_eval(rbc_condition self);

RBC_END_EXTERN_C
