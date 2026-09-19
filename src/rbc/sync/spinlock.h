#pragma once

#include <rbc/sync/common.h>

typedef struct RbcSpinLockImpl RbcSpinLockImpl;

struct RBC_CAPABILITY("RbcSpinLock") RbcSpinLock {
	RbcSpinLockImpl* impl;
};
typedef struct RbcSpinLock RbcSpinLock;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_spinlock_init(RbcSpinLock* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT RbcError rbc_spinlock_destroy(RbcSpinLock* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT RbcError rbc_spinlock_lock(RbcSpinLock self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT RbcError rbc_spinlock_try_lock(RbcSpinLock self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT RbcError rbc_spinlock_unlock(RbcSpinLock self) RBC_RELEASE_CAPABILITY(self);

RBC_END_EXTERN_C
