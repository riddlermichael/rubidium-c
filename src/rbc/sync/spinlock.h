#pragma once

#include <rbc/sync/common.h>

typedef struct rbc_spinlock_impl rbc_spinlock_impl;

struct RBC_CAPABILITY("rbc_spinlock") rbc_spinlock {
	rbc_spinlock_impl* impl;
};
typedef struct rbc_spinlock rbc_spinlock;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_spinlock_init(rbc_spinlock* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT rbc_error rbc_spinlock_destroy(rbc_spinlock* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT rbc_error rbc_spinlock_lock(rbc_spinlock self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT rbc_error rbc_spinlock_try_lock(rbc_spinlock self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);
RBC_EXPORT rbc_error rbc_spinlock_unlock(rbc_spinlock self) RBC_RELEASE_CAPABILITY(self);

RBC_END_EXTERN_C
