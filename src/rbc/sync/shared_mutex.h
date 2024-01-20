#pragma once

#include <rbc/sync/common.h>

typedef struct rbc_shared_mutex_impl rbc_shared_mutex_impl;

struct RBC_CAPABILITY("rbc_shared_mutex") rbc_shared_mutex {
	rbc_shared_mutex_impl* impl;
};
typedef struct rbc_shared_mutex rbc_shared_mutex;

enum rbc_shared_mutex_kind {
	RBC_SHARED_MUTEX_KIND_PREFER_READERS,
	RBC_SHARED_MUTEX_KIND_PREFER_WRITERS,
	RBC_SHARED_MUTEX_KIND_PREFER_WRITERS_NONRECURSIVE,
};
typedef enum rbc_shared_mutex_kind rbc_shared_mutex_kind;

RBC_BEGIN_EXTERN_C

RBC_EXPORT rbc_error rbc_shared_mutex_init(rbc_shared_mutex* self, rbc_shared_mutex_kind kind) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT rbc_error rbc_shared_mutex_destroy(rbc_shared_mutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT rbc_error rbc_shared_mutex_lock(rbc_shared_mutex self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT rbc_error rbc_shared_mutex_try_lock(rbc_shared_mutex self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);

RBC_EXPORT rbc_error rbc_shared_mutex_lock_shared(rbc_shared_mutex self) RBC_ACQUIRE_SHARED_CAPABILITY(self);
RBC_EXPORT rbc_error rbc_shared_mutex_try_lock_shared(rbc_shared_mutex self) RBC_TRY_ACQUIRE_SHARED_CAPABILITY(0, self);

RBC_EXPORT rbc_error rbc_shared_mutex_unlock(rbc_shared_mutex self) RBC_RELEASE_GENERIC_CAPABILITY(self);
RBC_EXPORT rbc_error rbc_shared_mutex_unlock_shared(rbc_shared_mutex self) RBC_RELEASE_SHARED_CAPABILITY(self);

RBC_END_EXTERN_C
