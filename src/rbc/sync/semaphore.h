#pragma once

#include <rbc/sync/common.h>

typedef struct RbcSemaphoreImpl RbcSemaphoreImpl;

struct RbcSemaphore {
	RbcSemaphoreImpl* impl;
};
typedef struct RbcSemaphore RbcSemaphore;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_semaphore_init(RbcSemaphore* self, unsigned count) RBC_NONNULL;
RBC_EXPORT RbcError rbc_semaphore_destroy(RbcSemaphore* self) RBC_NONNULL;

RBC_EXPORT RbcError rbc_semaphore_acquire(RbcSemaphore self);
RBC_EXPORT RbcError rbc_semaphore_try_acquire(RbcSemaphore self);
RBC_EXPORT RbcError rbc_semaphore_release(RbcSemaphore self);

RBC_END_EXTERN_C
