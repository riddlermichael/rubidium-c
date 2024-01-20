#include "recursive_mutex.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct rbc_recursive_mutex_impl {
	pthread_mutex_t impl;
};

rbc_error rbc_recursive_mutex_init(rbc_recursive_mutex* self) {
	#define RBC_SYNC_CHECK_WITH_CLEANUP(expr)     \
		do {                                      \
			int const _err = expr;                \
			if (_err) {                           \
				pthread_mutexattr_destroy(&attr); \
				return _err;                      \
			}                                     \
		} while (0)

	pthread_mutexattr_t attr;
	RBC_SYNC_CHECK(pthread_mutexattr_init(&attr));
	RBC_SYNC_CHECK_WITH_CLEANUP(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));

	#if !defined(RBC_COMPILER_MINGW) && (!defined(RBC_COMPILER_GCC) || !defined(RBC_OS_CYGWIN))
	RBC_SYNC_CHECK_WITH_CLEANUP(pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST));
	#endif

	self->impl = malloc(sizeof(rbc_recursive_mutex_impl));
	RBC_SYNC_CHECK_WITH_CLEANUP(self->impl ? RBC_ERROR_OK : RBC_ERROR_NOT_ENOUGH_MEMORY);

	int const error = pthread_mutex_init(RBC_SYNC_IMPL_PTR, &attr);
	pthread_mutexattr_destroy(&attr);
	return error;

	#undef RBC_SYNC_CHECK_WITH_CLEANUP
}

rbc_error rbc_recursive_mutex_destroy(rbc_recursive_mutex* self) {
	RBC_SYNC_DESTROY(pthread_mutex_destroy(RBC_SYNC_IMPL_PTR));
}

rbc_error rbc_recursive_mutex_lock(rbc_recursive_mutex self) {
	return pthread_mutex_lock(RBC_SYNC_IMPL);
}

rbc_error rbc_recursive_mutex_lock_for(rbc_recursive_mutex self, rbc_duration timeout) {
	rbc_time const deadline = rbc_time_deadline_from_timeout(timeout);
	return rbc_recursive_mutex_lock_until(self, deadline);
}

rbc_error rbc_recursive_mutex_lock_until(rbc_recursive_mutex self, rbc_time deadline) {
	rbc_timespec const ts = rbc_time_to_timespec(deadline);
	return pthread_mutex_timedlock(RBC_SYNC_IMPL, (struct timespec const*) &ts);
}

rbc_error rbc_recursive_mutex_try_lock(rbc_recursive_mutex self) {
	return pthread_mutex_trylock(RBC_SYNC_IMPL);
}

rbc_error rbc_recursive_mutex_unlock(rbc_recursive_mutex self) {
	return pthread_mutex_unlock(RBC_SYNC_IMPL);
}

#endif
