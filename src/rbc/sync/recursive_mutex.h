#pragma once

#include <rbc/sync/common.h>
#include <rbc/time/time.h>

typedef struct RbcRecursiveMutexImpl RbcRecursiveMutexImpl;

struct RBC_CAPABILITY("RbcRecursiveMutex") RbcRecursiveMutex {
	RbcRecursiveMutexImpl* impl;
};
typedef struct RbcRecursiveMutex RbcRecursiveMutex;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_recursive_mutex_init(RbcRecursiveMutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT RbcError rbc_recursive_mutex_destroy(RbcRecursiveMutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT RbcError rbc_recursive_mutex_lock(RbcRecursiveMutex self);
RBC_EXPORT RbcError rbc_recursive_mutex_lock_for(RbcRecursiveMutex self, RbcDuration timeout);
RBC_EXPORT RbcError rbc_recursive_mutex_lock_until(RbcRecursiveMutex self, RbcTime deadline);
RBC_EXPORT RbcError rbc_recursive_mutex_try_lock(RbcRecursiveMutex self);
RBC_EXPORT RbcError rbc_recursive_mutex_unlock(RbcRecursiveMutex self);

RBC_END_EXTERN_C
