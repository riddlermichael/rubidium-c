#pragma once

#include <rbc/sync/common.h>

typedef struct rbc_semaphore_impl rbc_semaphore_impl;

struct rbc_semaphore {
	rbc_semaphore_impl* impl;
};
typedef struct rbc_semaphore rbc_semaphore;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_semaphore_init(rbc_semaphore* self, unsigned count) RBC_NONNULL;
RBC_EXPORT rbc_error rbc_semaphore_destroy(rbc_semaphore* self) RBC_NONNULL;

RBC_EXPORT rbc_error rbc_semaphore_acquire(rbc_semaphore self);
RBC_EXPORT rbc_error rbc_semaphore_try_acquire(rbc_semaphore self);
RBC_EXPORT rbc_error rbc_semaphore_release(rbc_semaphore self);

RBC_END_EXTERN_C
