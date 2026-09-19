#pragma once

#include <rbc/sync/cond_var.h>

struct RBC_CAPABILITY("RbcConditionMutex") RbcConditionMutex {
	RbcMutex mu;
	RbcCondVar cv;
};
typedef struct RbcConditionMutex RbcConditionMutex;

RBC_BEGIN_EXTERN_C
typedef bool (*RbcConditionFn)(void*);
RBC_END_EXTERN_C

struct RbcCondition {
	RbcConditionFn fn;
	void* arg;
};
typedef struct RbcCondition RbcCondition;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_condition_mutex_init(RbcConditionMutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT RbcError rbc_condition_mutex_destroy(RbcConditionMutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT RbcError rbc_condition_mutex_lock(RbcConditionMutex self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT RbcError rbc_condition_mutex_lock_for(RbcConditionMutex self, RbcDuration timeout)
    RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT RbcError rbc_condition_mutex_lock_until(RbcConditionMutex self, RbcTime deadline)
    RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT RbcError rbc_condition_mutex_try_lock(RbcConditionMutex self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT RbcError rbc_condition_mutex_unlock(RbcConditionMutex self) RBC_RELEASE_CAPABILITY(self);

RBC_EXPORT RbcError rbc_condition_mutex_await(RbcConditionMutex self, RbcCondition condition);
RBC_EXPORT RbcError rbc_condition_mutex_await_for(RbcConditionMutex self, RbcCondition condition, RbcDuration timeout);
RBC_EXPORT RbcError rbc_condition_mutex_await_until(RbcConditionMutex self, RbcCondition condition, RbcTime deadline);
RBC_EXPORT RbcError rbc_condition_mutex_lock_when(RbcConditionMutex self, RbcCondition condition) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT RbcError rbc_condition_mutex_lock_when_for(RbcConditionMutex self, RbcCondition condition, RbcDuration timeout);
RBC_EXPORT RbcError rbc_condition_mutex_lock_when_until(RbcConditionMutex self, RbcCondition condition, RbcTime deadline);

/// Make RbcCondition that returns the result of `(*fn)(arg)`.
/// Equivalent of `(RbcCondition){fn, arg}`.
RBC_EXPORT RbcCondition rbc_condition_from_fn(RbcConditionFn fn, void* arg);

/// Make RbcCondition that returns the value of `*cond`.
RBC_EXPORT RbcCondition rbc_condition_from_bool(bool const* cond);

/// Evaluates the condition.
RBC_EXPORT bool rbc_condition_eval(RbcCondition self);

RBC_END_EXTERN_C
