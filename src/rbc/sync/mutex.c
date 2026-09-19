#include "mutex.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct RbcMutexImpl {
	pthread_mutex_t impl;
};

RbcError rbc_mutex_init(RbcMutex* self) {
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
	RBC_SYNC_CHECK_WITH_CLEANUP(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK));

	#if !defined(RBC_COMPILER_MINGW) && (!defined(RBC_COMPILER_GCC) || !defined(RBC_OS_CYGWIN)) && !defined(RBC_OS_DARWIN)
	RBC_SYNC_CHECK_WITH_CLEANUP(pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST));
	#endif

	self->impl = malloc(sizeof(RbcMutexImpl));
	RBC_SYNC_CHECK_WITH_CLEANUP(self->impl ? RBC_ERROR_OK : RBC_ERROR_NOT_ENOUGH_MEMORY);

	int const error = pthread_mutex_init(RBC_SYNC_IMPL_PTR, &attr);
	pthread_mutexattr_destroy(&attr);
	return error;

	#undef RBC_SYNC_CHECK_WITH_CLEANUP
}

RbcError rbc_mutex_destroy(RbcMutex* self) {
	RBC_SYNC_DESTROY(pthread_mutex_destroy(RBC_SYNC_IMPL_PTR));
}

RbcError rbc_mutex_lock(RbcMutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_mutex_lock(RBC_SYNC_IMPL);
}

RbcError rbc_mutex_lock_for(RbcMutex self, RbcDuration timeout) RBC_NO_THREAD_SAFETY_ANALYSIS {
	RbcTime const deadline = rbc_time_deadline_from_timeout(timeout);
	return rbc_mutex_lock_until(self, deadline);
}

RbcError rbc_mutex_lock_until(RbcMutex self, RbcTime deadline) RBC_NO_THREAD_SAFETY_ANALYSIS {
	#ifndef RBC_OS_DARWIN
	RbcTimespec const ts = rbc_time_to_timespec(deadline);
	return pthread_mutex_timedlock(RBC_SYNC_IMPL, (struct timespec const*) &ts);
	#else
	RBC_UNUSED(self);
	RBC_UNUSED(deadline);
	return RBC_ERROR_NOT_IMPLEMENTED;
	#endif
}

RbcError rbc_mutex_try_lock(RbcMutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_mutex_trylock(RBC_SYNC_IMPL);
}

RbcError rbc_mutex_unlock(RbcMutex self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_mutex_unlock(RBC_SYNC_IMPL);
}

#elif RBC_USE(WIN32_THREADS)

struct RbcMutexImpl {
	CRITICAL_SECTION impl;
};

RbcError rbc_mutex_init(RbcMutex* self) {
	RBC_SYNC_INIT(RbcMutex);
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

RbcError rbc_mutex_destroy(RbcMutex* self) {
	if (self->impl) {
		DeleteCriticalSection(RBC_SYNC_IMPL_PTR);
		free(self->impl);
	}
	return RBC_ERROR_OK;
}

RbcError rbc_mutex_lock(RbcMutex self) {
	/**
	 * This function can raise EXCEPTION_POSSIBLE_DEADLOCK, also known as STATUS_POSSIBLE_DEADLOCK,
	 * if a wait operation on the critical section times out.
	 * The timeout interval is specified by the following registry value:
	 * HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\CriticalSectionTimeout.
	 */
	__try {
		EnterCriticalSection(RBC_SYNC_IMPL);
		// ReSharper disable once CppDFAUnreachableCode
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return RBC_ERROR_RESOURCE_DEADLOCK_WOULD_OCCUR;
	}
	return RBC_ERROR_OK;
}

RbcError rbc_mutex_lock_for(RbcMutex self, RbcDuration timeout) {
	RBC_UNUSED(self);
	RBC_UNUSED(timeout);
	return RBC_ERROR_NOT_IMPLEMENTED;
}

RbcError rbc_mutex_lock_until(RbcMutex self, RbcTime deadline) {
	RBC_UNUSED(self);
	RBC_UNUSED(deadline);
	return RBC_ERROR_NOT_IMPLEMENTED;
}

RbcError rbc_mutex_try_lock(RbcMutex self) {
	return TryEnterCriticalSection(RBC_SYNC_IMPL)
	    ? RBC_ERROR_OK
	    : RBC_ERROR_DEVICE_OR_RESOURCE_BUSY;
}

RbcError rbc_mutex_unlock(RbcMutex self) {
	LeaveCriticalSection(RBC_SYNC_IMPL);
	return RBC_ERROR_OK;
}

#endif

// RBC_LOCK_GUARD_IMPL(RbcMutexLocker, RbcMutex)
