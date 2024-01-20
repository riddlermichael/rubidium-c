#include "shared_mutex.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct rbc_shared_mutex_impl {
	pthread_rwlock_t impl;
};

rbc_error rbc_shared_mutex_init(rbc_shared_mutex* self, rbc_shared_mutex_kind kind) {
	#define RBC_SYNC_CHECK_WITH_CLEANUP(expr)      \
		do {                                       \
			int const _err = expr;                 \
			if (_err) {                            \
				pthread_rwlockattr_destroy(&attr); \
				return _err;                       \
			}                                      \
		} while (0)

	pthread_rwlockattr_t attr;
	RBC_SYNC_CHECK(pthread_rwlockattr_init(&attr));

	#ifdef RB_OS_WIN
	RBC_UNUSED(kind);
	#else
	RBC_SYNC_CHECK_WITH_CLEANUP(pthread_rwlockattr_setkind_np(&attr, kind));
	#endif

	self->impl = malloc(sizeof(rbc_shared_mutex_impl));
	RBC_SYNC_CHECK_WITH_CLEANUP(self->impl ? RBC_ERROR_OK : RBC_ERROR_NOT_ENOUGH_MEMORY);

	int const error = pthread_rwlock_init(RBC_SYNC_IMPL_PTR, &attr);
	pthread_rwlockattr_destroy(&attr);
	return error;

	#undef RBC_SYNC_CHECK_WITH_CLEANUP
}

rbc_error rbc_shared_mutex_destroy(rbc_shared_mutex* self) {
	RBC_SYNC_DESTROY(pthread_rwlock_destroy(RBC_SYNC_IMPL_PTR));
}

rbc_error rbc_shared_mutex_lock(rbc_shared_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_rwlock_wrlock(RBC_SYNC_IMPL);
}

rbc_error rbc_shared_mutex_try_lock(rbc_shared_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_rwlock_trywrlock(RBC_SYNC_IMPL);
}

rbc_error rbc_shared_mutex_lock_shared(rbc_shared_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_rwlock_rdlock(RBC_SYNC_IMPL);
}

rbc_error rbc_shared_mutex_try_lock_shared(rbc_shared_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_rwlock_tryrdlock(RBC_SYNC_IMPL);
}

rbc_error rbc_shared_mutex_unlock(rbc_shared_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_rwlock_unlock(RBC_SYNC_IMPL);
}

rbc_error rbc_shared_mutex_unlock_shared(rbc_shared_mutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_rwlock_unlock(RBC_SYNC_IMPL);
}

#endif
