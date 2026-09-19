#include "semaphore.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

	#include <semaphore.h> // NOLINT(*-duplicate-include)

	#include <rbc/core/warnings.h>

struct RbcSemaphoreImpl {
	sem_t impl;
};

RBC_WARNING_PUSH
RBC_WARNING_DEPRECATED

// NOLINTBEGIN(clang-diagnostic-deprecated-declarations)
// ReSharper disable CppDeprecatedEntity

RbcError rbc_semaphore_init(RbcSemaphore* self, unsigned count) {
	RBC_SYNC_INIT(RbcSemaphore);
	RBC_SYNC_CHECK_LAST_ERROR(sem_init(RBC_SYNC_IMPL_PTR, false, count));
}

RbcError rbc_semaphore_destroy(RbcSemaphore* self) {
	if (!self->impl) {
		return RBC_ERROR_OK;
	}

	int const error = sem_destroy(RBC_SYNC_IMPL_PTR);
	free(self->impl);
	self->impl = NULL;
	return error ? errno : RBC_ERROR_OK;
}

// NOLINTEND(clang-diagnostic-deprecated-declarations)
RBC_WARNING_POP

RbcError rbc_semaphore_acquire(RbcSemaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(sem_wait(RBC_SYNC_IMPL));
}

RbcError rbc_semaphore_try_acquire(RbcSemaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(sem_trywait(RBC_SYNC_IMPL));
}

RbcError rbc_semaphore_release(RbcSemaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(sem_post(RBC_SYNC_IMPL));
}

#elif RBC_USE(WIN32_THREADS)

struct RbcSemaphoreImpl {
	HANDLE impl;
};

RbcError rbc_semaphore_init(RbcSemaphore* self, unsigned count) {
	RBC_SYNC_INIT(RbcSemaphore);
	RBC_SYNC_DEREF_IMPL_PTR = CreateSemaphoreA(NULL, count, count, NULL);
	return RBC_SYNC_DEREF_IMPL_PTR ? RBC_ERROR_OK : rbc_error_from_last_error();
}

RbcError rbc_semaphore_destroy(RbcSemaphore* self) {
	RBC_SYNC_DESTROY(CloseHandle(RBC_SYNC_DEREF_IMPL_PTR));
}

RbcError rbc_semaphore_acquire(RbcSemaphore self) {
	DWORD const rc = WaitForSingleObject(RBC_SYNC_DEREF_IMPL, INFINITE);
	switch (rc) {
		case WAIT_FAILED  : return rbc_error_from_last_error();
		case WAIT_OBJECT_0: return RBC_ERROR_OK;
		default           : return RBC_ERROR_UNKNOWN;
	}
}

RbcError rbc_semaphore_try_acquire(RbcSemaphore self) {
	DWORD const rc = WaitForSingleObject(RBC_SYNC_DEREF_IMPL, 0);
	switch (rc) {
		case WAIT_FAILED  : return rbc_error_from_last_error();
		case WAIT_OBJECT_0: return RBC_ERROR_OK;
		case WAIT_TIMEOUT : return RBC_ERROR_RESOURCE_UNAVAILABLE_TRY_AGAIN;
		default           : return RBC_ERROR_UNKNOWN;
	}
}

RbcError rbc_semaphore_release(RbcSemaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(ReleaseSemaphore(RBC_SYNC_DEREF_IMPL, 1, NULL));
}

#endif
