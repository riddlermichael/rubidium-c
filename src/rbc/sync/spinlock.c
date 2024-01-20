#include "spinlock.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct rbc_spinlock_impl {
	pthread_spinlock_t impl;
};

rbc_error rbc_spinlock_init(rbc_spinlock* self) {
	RBC_SYNC_INIT(rbc_spinlock);
	RBC_SYNC_CHECK_INIT(pthread_spin_init(RBC_SYNC_IMPL_PTR, false));
}

rbc_error rbc_spinlock_destroy(rbc_spinlock* self) {
	RBC_SYNC_DESTROY(pthread_spin_destroy(RBC_SYNC_IMPL_PTR));
}

rbc_error rbc_spinlock_lock(rbc_spinlock self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_spin_lock(RBC_SYNC_IMPL);
}

rbc_error rbc_spinlock_try_lock(rbc_spinlock self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_spin_trylock(RBC_SYNC_IMPL);
}

rbc_error rbc_spinlock_unlock(rbc_spinlock self) RBC_NO_THREAD_SAFETY_ANALYSIS {
	return pthread_spin_unlock(RBC_SYNC_IMPL);
}

#endif
