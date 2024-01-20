#include "cond_var.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct rbc_cond_var_impl {
	pthread_cond_t impl;
};

rbc_error rbc_cond_var_init(rbc_cond_var* self) {
	RBC_SYNC_INIT(rbc_cond_var);
	RBC_SYNC_CHECK_INIT(pthread_cond_init(RBC_SYNC_IMPL_PTR, NULL));
}

rbc_error rbc_cond_var_destroy(rbc_cond_var* self) {
	RBC_SYNC_DESTROY(pthread_cond_destroy(RBC_SYNC_IMPL_PTR));
}

rbc_error rbc_cond_var_notify(rbc_cond_var self) {
	return pthread_cond_signal(RBC_SYNC_IMPL);
}

rbc_error rbc_cond_var_notify_all(rbc_cond_var self) {
	return pthread_cond_broadcast(RBC_SYNC_IMPL);
}

rbc_error rbc_cond_var_wait(rbc_cond_var self, rbc_mutex mutex) {
	pthread_mutex_t* mtx = (pthread_mutex_t*) mutex.impl;
	return pthread_cond_wait(RBC_SYNC_IMPL, mtx);
}

rbc_error rbc_cond_var_wait_for(rbc_cond_var self, rbc_mutex mutex, rbc_duration timeout) {
	rbc_time const deadline = rbc_time_deadline_from_timeout(timeout);
	return rbc_cond_var_wait_until(self, mutex, deadline);
}

rbc_error rbc_cond_var_wait_until(rbc_cond_var self, rbc_mutex mutex, rbc_time deadline) {
	pthread_mutex_t* mtx = (pthread_mutex_t*) mutex.impl;
	rbc_timespec const ts = rbc_time_to_timespec(deadline);
	return pthread_cond_timedwait(RBC_SYNC_IMPL, mtx, (struct timespec const*) &ts);
}

#endif
