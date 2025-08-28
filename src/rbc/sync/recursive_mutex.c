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

	#if !defined(RBC_COMPILER_MINGW) && (!defined(RBC_COMPILER_GCC) || !defined(RBC_OS_CYGWIN)) && !defined(RBC_OS_DARWIN)
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
	#ifdef RBC_OS_DARWIN
	RBC_UNUSED(self);
	RBC_UNUSED(deadline);
	return RBC_ERROR_NOT_IMPLEMENTED;
	#else
	rbc_timespec const ts = rbc_time_to_timespec(deadline);
	return pthread_mutex_timedlock(RBC_SYNC_IMPL, (struct timespec const*) &ts);
	#endif
}

rbc_error rbc_recursive_mutex_try_lock(rbc_recursive_mutex self) {
	return pthread_mutex_trylock(RBC_SYNC_IMPL);
}

rbc_error rbc_recursive_mutex_unlock(rbc_recursive_mutex self) {
	return pthread_mutex_unlock(RBC_SYNC_IMPL);
}

#elif RBC_USE(WIN32_THREADS)

struct rbc_recursive_mutex_impl {
	CRITICAL_SECTION impl;
};

rbc_error rbc_recursive_mutex_init(rbc_recursive_mutex* self) {
	RBC_SYNC_INIT(rbc_recursive_mutex);

	/**
	 * Windows Server 2003 and Windows XP:
	 * In low memory situations, InitializeCriticalSection can raise a STATUS_NO_MEMORY exception.
	 * Starting with Windows Vista, this exception was eliminated and InitializeCriticalSection always succeeds,
	 * even in low memory situations.
	 */
	#if _WIN32_WINNT >= 0x0600
	InitializeCriticalSection(RBC_SYNC_IMPL_PTR);
	#else
	__try {
		InitializeCriticalSection(RBC_SYNC_IMPL_PTR);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return RBC_ERROR_NOT_ENOUGH_MEMORY;
	}
	#endif
	return RBC_ERROR_OK;
}

rbc_error rbc_recursive_mutex_destroy(rbc_recursive_mutex* self) {
	DeleteCriticalSection(RBC_SYNC_IMPL_PTR);
	RBC_SYNC_DESTROY(RBC_ERROR_OK);
}

rbc_error rbc_recursive_mutex_lock(rbc_recursive_mutex self) {
	/**
	 * This function can raise EXCEPTION_POSSIBLE_DEADLOCK, also known as STATUS_POSSIBLE_DEADLOCK,
	 * if a wait operation on the critical section times out.
	 * The timeout interval is specified by the following registry value:
	 * HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\CriticalSectionTimeout.
	 */
	__try {
		EnterCriticalSection(RBC_SYNC_IMPL);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return RBC_ERROR_RESOURCE_DEADLOCK_WOULD_OCCUR;
	}
	return RBC_ERROR_OK;
}

rbc_error rbc_recursive_mutex_lock_for(rbc_recursive_mutex self, rbc_duration timeout) {
	RBC_UNUSED(self);
	RBC_UNUSED(timeout);
	return RBC_ERROR_NOT_IMPLEMENTED;
}

rbc_error rbc_recursive_mutex_lock_until(rbc_recursive_mutex self, rbc_time deadline) {
	RBC_UNUSED(self);
	RBC_UNUSED(deadline);
	return RBC_ERROR_NOT_IMPLEMENTED;
}

rbc_error rbc_recursive_mutex_try_lock(rbc_recursive_mutex self) {
	return TryEnterCriticalSection(RBC_SYNC_IMPL)
	         ? RBC_ERROR_OK
	         : RBC_ERROR_DEVICE_OR_RESOURCE_BUSY;
}

rbc_error rbc_recursive_mutex_unlock(rbc_recursive_mutex self) {
	LeaveCriticalSection(RBC_SYNC_IMPL);
	return RBC_ERROR_OK;
}

#endif
