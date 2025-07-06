#include "barrier.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

#ifdef _POSIX_BARRIERS

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
	return (!error || error == PTHREAD_BARRIER_SERIAL_THREAD) ? RBC_ERROR_OK : error;
}

#endif

#elif RBC_USE(WIN32_THREADS)

struct rbc_barrier_impl {
	SYNCHRONIZATION_BARRIER impl;
};

rbc_error rbc_barrier_init(rbc_barrier* self, unsigned count) {
	RBC_SYNC_INIT(rbc_barrier);
	/**
	 * BOOL InitializeSynchronizationBarrier(
	 * [out] LPSYNCHRONIZATION_BARRIER lpBarrier,
	 * [in]  LONG                      lTotalThreads,
	 * [in]  LONG                      lSpinCount
	 * );
	 *
	 * [in] lSpinCount
	 * The number of times an individual thread should spin while waiting for other threads to arrive at the barrier.
	 * If this parameter is -1, the thread spins 2000 times.
	 * If the thread exceeds lSpinCount, the thread blocks unless it called EnterSynchronizationBarrier
	 * with SYNCHRONIZATION_BARRIER_FLAGS_SPIN_ONLY.
	 */
	RBC_SYNC_CHECK_LAST_ERROR(InitializeSynchronizationBarrier(RBC_SYNC_IMPL_PTR, count, -1));
}

rbc_error rbc_barrier_destroy(rbc_barrier* self) {
	// the DeleteSynchronizationBarrier function always returns TRUE
	DeleteSynchronizationBarrier(RBC_SYNC_IMPL_PTR);
	RBC_SYNC_DESTROY(RBC_ERROR_OK);
}

rbc_error rbc_barrier_wait(rbc_barrier self) {
	// TRUE for the last thread to signal the barrier.
	// Threads that signal the barrier before the last thread signals it receive a return value of FALSE.
	EnterSynchronizationBarrier(RBC_SYNC_IMPL, 0);
	return RBC_ERROR_OK;
}

#endif
