#pragma once

#include <rbc/sync/common.h>

typedef struct RbcSharedMutexImpl RbcSharedMutexImpl;

struct RBC_CAPABILITY("RbcSharedMutex") RbcSharedMutex {
	RbcSharedMutexImpl* impl;
};
typedef struct RbcSharedMutex RbcSharedMutex;

enum RbcSharedMutexKind {
	RBC_SHARED_MUTEX_KIND_PREFER_READERS,
	RBC_SHARED_MUTEX_KIND_PREFER_WRITERS,
	RBC_SHARED_MUTEX_KIND_PREFER_WRITERS_NONRECURSIVE,
};
typedef enum RbcSharedMutexKind RbcSharedMutexKind;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RbcError rbc_shared_mutex_init(RbcSharedMutex* self, RbcSharedMutexKind kind) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);
RBC_EXPORT RbcError rbc_shared_mutex_destroy(RbcSharedMutex* self) RBC_NONNULL RBC_LOCKS_EXCLUDED(*self);

RBC_EXPORT RbcError rbc_shared_mutex_lock(RbcSharedMutex self) RBC_ACQUIRE_CAPABILITY(self);
RBC_EXPORT RbcError rbc_shared_mutex_try_lock(RbcSharedMutex self) RBC_TRY_ACQUIRE_CAPABILITY(0, self);

RBC_EXPORT RbcError rbc_shared_mutex_lock_shared(RbcSharedMutex self) RBC_ACQUIRE_SHARED_CAPABILITY(self);
RBC_EXPORT RbcError rbc_shared_mutex_try_lock_shared(RbcSharedMutex self) RBC_TRY_ACQUIRE_SHARED_CAPABILITY(0, self);

RBC_EXPORT RbcError rbc_shared_mutex_unlock(RbcSharedMutex self) RBC_RELEASE_GENERIC_CAPABILITY(self);
RBC_EXPORT RbcError rbc_shared_mutex_unlock_shared(RbcSharedMutex self) RBC_RELEASE_SHARED_CAPABILITY(self);

RBC_END_EXTERN_C
