#include "cond_var.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct RbcCondVarImpl {
	pthread_cond_t impl;
};

RbcError rbc_cond_var_init(RbcCondVar* self) {
	RBC_SYNC_INIT(RbcCondVar);
	RBC_SYNC_CHECK_INIT(pthread_cond_init(RBC_SYNC_IMPL_PTR, NULL));
}

RbcError rbc_cond_var_destroy(RbcCondVar* self) {
	RBC_SYNC_DESTROY(pthread_cond_destroy(RBC_SYNC_IMPL_PTR));
}

RbcError rbc_cond_var_notify(RbcCondVar self) {
	return pthread_cond_signal(RBC_SYNC_IMPL);
}

RbcError rbc_cond_var_notify_all(RbcCondVar self) {
	return pthread_cond_broadcast(RBC_SYNC_IMPL);
}

RbcError rbc_cond_var_wait(RbcCondVar self, RbcMutex mutex) {
	pthread_mutex_t* mtx = (pthread_mutex_t*) mutex.impl;
	return pthread_cond_wait(RBC_SYNC_IMPL, mtx);
}

RbcError rbc_cond_var_wait_for(RbcCondVar self, RbcMutex mutex, RbcDuration timeout) {
	RbcTime const deadline = rbc_time_deadline_from_timeout(timeout);
	return rbc_cond_var_wait_until(self, mutex, deadline);
}

RbcError rbc_cond_var_wait_until(RbcCondVar self, RbcMutex mutex, RbcTime deadline) {
	pthread_mutex_t* mtx = (pthread_mutex_t*) mutex.impl;
	RbcTimespec const ts = rbc_time_to_timespec(deadline);
	return pthread_cond_timedwait(RBC_SYNC_IMPL, mtx, (struct timespec const*) &ts);
}

#elif RBC_USE(WIN32_THREADS)

struct RbcCondVarImpl {
	CONDITION_VARIABLE impl;
};

RbcError rbc_cond_var_init(RbcCondVar* self) {
	RBC_SYNC_INIT(RbcCondVar);
	InitializeConditionVariable(RBC_SYNC_IMPL_PTR);
	return RBC_ERROR_OK;
}

RbcError rbc_cond_var_destroy(RbcCondVar* self) {
	// see https://joeduffyblog.com/2006/11/28/windows-keyed-events-critical-sections-and-new-vista-synchronization-features/
	RBC_SYNC_DESTROY(RBC_ERROR_OK);
}

RbcError rbc_cond_var_notify(RbcCondVar self) {
	WakeConditionVariable(RBC_SYNC_IMPL);
	return RBC_ERROR_OK;
}

RbcError rbc_cond_var_notify_all(RbcCondVar self) {
	WakeAllConditionVariable(RBC_SYNC_IMPL);
	return RBC_ERROR_OK;
}

RbcError rbc_cond_var_wait(RbcCondVar self, RbcMutex mutex) {
	CRITICAL_SECTION* cs = (CRITICAL_SECTION*) mutex.impl;
	RBC_SYNC_CHECK_LAST_ERROR(SleepConditionVariableCS(RBC_SYNC_IMPL, cs, INFINITE));
}

RbcError rbc_cond_var_wait_for(RbcCondVar self, RbcMutex mutex, RbcDuration timeout) {
	RBC_UNUSED(self);
	RBC_UNUSED(mutex);
	RBC_UNUSED(timeout);
	return RBC_ERROR_NOT_IMPLEMENTED;
}

RbcError rbc_cond_var_wait_until(RbcCondVar self, RbcMutex mutex, RbcTime deadline) {
	RBC_UNUSED(self);
	RBC_UNUSED(mutex);
	RBC_UNUSED(deadline);
	return RBC_ERROR_NOT_IMPLEMENTED;
}

#endif
