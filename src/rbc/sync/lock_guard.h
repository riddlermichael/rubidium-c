#pragma once

#include <rbc/containers/macros.h>
#include <rbc/core/attributes.h>
#include <rbc/core/export.h>
#include <rbc/core/extern.h>
#include <rbc/sync/annotations.h>

#define RBC_LOCK_GUARD_DEF(type_name, lockable_t, export)                                      \
	typedef lockable_t RBC_CONCAT(type_name, _lockable_t);                                     \
                                                                                               \
	struct RBC_SCOPED_CAPABILITY type_name {                                                   \
		lockable_t* lockable;                                                                  \
	};                                                                                         \
	typedef struct type_name type_name;                                                        \
                                                                                               \
	RBC_BEGIN_EXTERN_C                                                                         \
                                                                                               \
	export rbc_error RBC_METHOD(type_name, init)(type_name * self)                             \
	    RBC_NONNULL RBC_ACQUIRE_CAPABILITY(self->lockable);                                    \
	export rbc_error RBC_METHOD(type_name, init_with)(type_name * self, lockable_t * lockable) \
	    RBC_NONNULL RBC_ACQUIRE_CAPABILITY(self->lockable);                                    \
	export rbc_error RBC_METHOD(type_name, destroy)(type_name * self)                          \
	    RBC_NONNULL RBC_RELEASE_CAPABILITY(self->lockable);                                    \
                                                                                               \
	RBC_END_EXTERN_C

#define RBC_LOCK_GUARD_IMPL(type_name, lockable_t)                                      \
	rbc_error RBC_METHOD(type_name, init)(type_name * self)                             \
	    RBC_NO_THREAD_SAFETY_ANALYSIS {                                                 \
		return RBC_CONCAT(lockable_t, _lock)(*self->lockable);                          \
	}                                                                                   \
                                                                                        \
	rbc_error RBC_METHOD(type_name, init_with)(type_name * self, lockable_t * lockable) \
	    RBC_NO_THREAD_SAFETY_ANALYSIS {                                                 \
		self->lockable = lockable;                                                      \
		return RBC_CONCAT(lockable_t, _lock)(*lockable);                                \
	}                                                                                   \
                                                                                        \
	rbc_error RBC_METHOD(type_name, destroy)(type_name * self)                          \
	    RBC_NO_THREAD_SAFETY_ANALYSIS {                                                 \
		return RBC_CONCAT(lockable_t, _unlock)(*self->lockable);                        \
	}
