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
	RBC_SYNC_CHECK_ERRNO(sem_init(RBC_SYNC_IMPL_PTR, false, count));
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
	RBC_SYNC_CHECK_ERRNO(sem_wait(RBC_SYNC_IMPL));
}

rbc_error rbc_semaphore_try_acquire(rbc_semaphore self) {
	RBC_SYNC_CHECK_ERRNO(sem_trywait(RBC_SYNC_IMPL));
}

rbc_error rbc_semaphore_release(rbc_semaphore self) {
	RBC_SYNC_CHECK_ERRNO(sem_post(RBC_SYNC_IMPL));
}

#endif
