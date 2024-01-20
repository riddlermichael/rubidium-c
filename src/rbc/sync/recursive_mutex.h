#pragma once

#include <rbc/sync/common.h>
#include <rbc/time/time.h>

typedef struct rbc_recursive_mutex_impl rbc_recursive_mutex_impl;

struct RBC_CAPABILITY("rbc_recursive_mutex") rbc_recursive_mutex {
	rbc_recursive_mutex_impl* impl;
};
typedef struct rbc_recursive_mutex rbc_recursive_mutex;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_recursive_mutex_init(rbc_recursive_mutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT rbc_error rbc_recursive_mutex_destroy(rbc_recursive_mutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT rbc_error rbc_recursive_mutex_lock(rbc_recursive_mutex self);
RBC_EXPORT rbc_error rbc_recursive_mutex_lock_for(rbc_recursive_mutex self, rbc_duration timeout);
RBC_EXPORT rbc_error rbc_recursive_mutex_lock_until(rbc_recursive_mutex self, rbc_time deadline);
RBC_EXPORT rbc_error rbc_recursive_mutex_try_lock(rbc_recursive_mutex self);
RBC_EXPORT rbc_error rbc_recursive_mutex_unlock(rbc_recursive_mutex self);

RBC_END_EXTERN_C
