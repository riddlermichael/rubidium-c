#pragma once

#include <rbc/sync/common.h>
#include <rbc/sync/lock_guard.h>
#include <rbc/time/time.h>

typedef struct rbc_mutex_impl rbc_mutex_impl;

struct RBC_CAPABILITY("rbc_mutex") rbc_mutex {
	rbc_mutex_impl* impl;
};
typedef struct rbc_mutex rbc_mutex;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_mutex_init(rbc_mutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT rbc_error rbc_mutex_destroy(rbc_mutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT rbc_error rbc_mutex_lock(rbc_mutex self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT rbc_error rbc_mutex_lock_for(rbc_mutex self, rbc_duration timeout) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT rbc_error rbc_mutex_lock_until(rbc_mutex self, rbc_time deadline) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT rbc_error rbc_mutex_try_lock(rbc_mutex self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT rbc_error rbc_mutex_unlock(rbc_mutex self) RBC_RELEASE_CAPABILITY(self);

RBC_END_EXTERN_C

RBC_LOCK_GUARD_DEF(rbc_mutex_locker, rbc_mutex, RBC_EXPORT)
