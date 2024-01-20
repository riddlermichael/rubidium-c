#pragma once

#include <rbc/sync/mutex.h>

typedef struct rbc_cond_var_impl rbc_cond_var_impl;

struct rbc_cond_var {
	rbc_cond_var_impl* impl;
};
typedef struct rbc_cond_var rbc_cond_var;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_cond_var_init(rbc_cond_var* self) RBC_NONNULL;
RBC_EXPORT rbc_error rbc_cond_var_destroy(rbc_cond_var* self) RBC_NONNULL;

RBC_EXPORT rbc_error rbc_cond_var_notify(rbc_cond_var self);
RBC_EXPORT rbc_error rbc_cond_var_notify_all(rbc_cond_var self);
RBC_EXPORT rbc_error rbc_cond_var_wait(rbc_cond_var self, rbc_mutex mutex) RBC_REQUIRES_CAPABILITY(mutex);
RBC_EXPORT rbc_error rbc_cond_var_wait_for(rbc_cond_var self, rbc_mutex mutex, rbc_duration timeout) RBC_REQUIRES_CAPABILITY(mutex);
RBC_EXPORT rbc_error rbc_cond_var_wait_until(rbc_cond_var self, rbc_mutex mutex, rbc_time deadline) RBC_REQUIRES_CAPABILITY(mutex);

RBC_END_EXTERN_C
