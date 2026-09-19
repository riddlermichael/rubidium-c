#pragma once

#include <rbc/sync/common.h>

typedef struct RbcBarrierImpl RbcBarrierImpl;

struct RbcBarrier {
	RbcBarrierImpl* impl;
};
typedef struct RbcBarrier RbcBarrier;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_barrier_init(RbcBarrier* self, unsigned count) RBC_NONNULL;
RBC_EXPORT RbcError rbc_barrier_destroy(RbcBarrier* self) RBC_NONNULL;

RBC_EXPORT RbcError rbc_barrier_wait(RbcBarrier self);

RBC_END_EXTERN_C
