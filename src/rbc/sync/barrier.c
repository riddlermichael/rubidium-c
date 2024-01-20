#include "barrier.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct rbc_barrier_impl {
	pthread_barrier_t impl;
};

rbc_error rbc_barrier_init(rbc_barrier* self, unsigned count) {
	RBC_SYNC_INIT(rbc_barrier);
	RBC_SYNC_CHECK_INIT(pthread_barrier_init(RBC_SYNC_IMPL_PTR, NULL, count));
}

rbc_error rbc_barrier_destroy(rbc_barrier* self) {
	RBC_SYNC_DESTROY(pthread_barrier_destroy(RBC_SYNC_IMPL_PTR));
}

rbc_error rbc_barrier_wait(rbc_barrier self) {
	int const error = pthread_barrier_wait(RBC_SYNC_IMPL);
	return (error == PTHREAD_BARRIER_SERIAL_THREAD) ? RBC_ERROR_OK : error;
}

#endif
