#include "once.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

rbc_error rbc_call_once(rbc_once* self, rbc_once_fn fn) {
	return pthread_once(&self->impl, fn);
}

#elif RBC_USE(WIN32_THREADS)

static BOOL WINAPI init_once_fn(INIT_ONCE* init_once, void* arg, void** context) {
	rbc_once_fn const fn = (rbc_once_fn) arg;
	fn();
	return TRUE;
}

rbc_error rbc_call_once(rbc_once* self, rbc_once_fn fn) {
	RBC_SYNC_CHECK_LAST_ERROR(InitOnceExecuteOnce((INIT_ONCE*) &self->impl, init_once_fn, (void*) fn, NULL));
}

#endif
