#pragma once

#include <rbc/sync/common.h>

typedef struct rbc_barrier_impl rbc_barrier_impl;

struct rbc_barrier {
	rbc_barrier_impl* impl;
};
typedef struct rbc_barrier rbc_barrier;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_barrier_init(rbc_barrier* self, unsigned count) RBC_NONNULL;
RBC_EXPORT rbc_error rbc_barrier_destroy(rbc_barrier* self) RBC_NONNULL;

RBC_EXPORT rbc_error rbc_barrier_wait(rbc_barrier self);

RBC_END_EXTERN_C
