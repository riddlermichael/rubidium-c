#pragma once

#include <rbc/sync/common.h>
#include <rbc/sync/lock_guard.h>
#include <rbc/time/time.h>

typedef struct RbcMutexImpl RbcMutexImpl;

struct RBC_CAPABILITY("RbcMutex") RbcMutex {
	RbcMutexImpl* impl;
};
typedef struct RbcMutex RbcMutex;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_mutex_init(RbcMutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT RbcError rbc_mutex_destroy(RbcMutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT RbcError rbc_mutex_lock(RbcMutex self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT RbcError rbc_mutex_lock_for(RbcMutex self, RbcDuration timeout) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT RbcError rbc_mutex_lock_until(RbcMutex self, RbcTime deadline) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT RbcError rbc_mutex_try_lock(RbcMutex self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT RbcError rbc_mutex_unlock(RbcMutex self) RBC_RELEASE_CAPABILITY(self);

RBC_END_EXTERN_C

// RBC_LOCK_GUARD_DEF(RbcMutexLocker, RbcMutex, RBC_EXPORT)
