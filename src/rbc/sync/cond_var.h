#pragma once

#include <rbc/sync/mutex.h>

typedef struct RbcCondVarImpl RbcCondVarImpl;

struct RbcCondVar {
	RbcCondVarImpl* impl;
};
typedef struct RbcCondVar RbcCondVar;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_cond_var_init(RbcCondVar* self) RBC_NONNULL;
RBC_EXPORT RbcError rbc_cond_var_destroy(RbcCondVar* self) RBC_NONNULL;

RBC_EXPORT RbcError rbc_cond_var_notify(RbcCondVar self);
RBC_EXPORT RbcError rbc_cond_var_notify_all(RbcCondVar self);
RBC_EXPORT RbcError rbc_cond_var_wait(RbcCondVar self, RbcMutex mutex) RBC_REQUIRES_CAPABILITY(mutex);
RBC_EXPORT RbcError rbc_cond_var_wait_for(RbcCondVar self, RbcMutex mutex, RbcDuration timeout) RBC_REQUIRES_CAPABILITY(mutex);
RBC_EXPORT RbcError rbc_cond_var_wait_until(RbcCondVar self, RbcMutex mutex, RbcTime deadline) RBC_REQUIRES_CAPABILITY(mutex);

RBC_END_EXTERN_C
