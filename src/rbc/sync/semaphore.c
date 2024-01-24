#include "semaphore.h"

#include <rbc/core/os.h>
#include <rbc/sync/impl.h>

#if defined(RBC_OS_LINUX)

	#include <semaphore.h> // NOLINT(*-duplicate-include)

struct rbc_semaphore_impl {
	sem_t impl;
};

rbc_error rbc_semaphore_init(rbc_semaphore* self, unsigned count) {
	RBC_SYNC_INIT(rbc_semaphore);
	RBC_SYNC_CHECK_LAST_ERROR(sem_init(RBC_SYNC_IMPL_PTR, false, count));
}

rbc_error rbc_semaphore_destroy(rbc_semaphore* self) {
	if (!self->impl) {
		return RBC_ERROR_OK;
	}

	int const error = sem_destroy(RBC_SYNC_IMPL_PTR);
	free(self->impl);
	self->impl = NULL;
	return error ? errno : RBC_ERROR_OK;
}

rbc_error rbc_semaphore_acquire(rbc_semaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(sem_wait(RBC_SYNC_IMPL));
}

rbc_error rbc_semaphore_try_acquire(rbc_semaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(sem_trywait(RBC_SYNC_IMPL));
}

rbc_error rbc_semaphore_release(rbc_semaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(sem_post(RBC_SYNC_IMPL));
}

#elif RBC_USE(WIN32_THREADS)

struct rbc_semaphore_impl {
	HANDLE impl;
};

rbc_error rbc_semaphore_init(rbc_semaphore* self, unsigned count) {
	RBC_SYNC_INIT(rbc_semaphore);
	RBC_SYNC_DEREF_IMPL_PTR = CreateSemaphoreA(NULL, count, count, NULL);
	return RBC_SYNC_DEREF_IMPL_PTR ? RBC_ERROR_OK : rbc_error_from_last_error();
}

rbc_error rbc_semaphore_destroy(rbc_semaphore* self) {
	RBC_SYNC_DESTROY(CloseHandle(RBC_SYNC_DEREF_IMPL_PTR));
}

rbc_error rbc_semaphore_acquire(rbc_semaphore self) {
	DWORD const rc = WaitForSingleObject(RBC_SYNC_DEREF_IMPL, INFINITE);
	switch (rc) {
		case WAIT_OBJECT_0: return RBC_ERROR_OK;
		case WAIT_FAILED  : return rbc_error_from_last_error();
		default           : return RBC_ERROR_UNKNOWN;
	}
}

rbc_error rbc_semaphore_try_acquire(rbc_semaphore self) {
	DWORD const rc = WaitForSingleObject(RBC_SYNC_DEREF_IMPL, 0);
	switch (rc) {
		case WAIT_OBJECT_0: return RBC_ERROR_OK;
		case WAIT_FAILED  : return rbc_error_from_last_error();
		case WAIT_TIMEOUT : return RBC_ERROR_RESOURCE_UNAVAILABLE_TRY_AGAIN;
		default           : return RBC_ERROR_UNKNOWN;
	}
}

rbc_error rbc_semaphore_release(rbc_semaphore self) {
	RBC_SYNC_CHECK_LAST_ERROR(ReleaseSemaphore(RBC_SYNC_DEREF_IMPL, 1, NULL));
}

#endif
