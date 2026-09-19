#include "once.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

RbcError rbc_call_once(RbcOnce* self, RbcOnceFn fn) {
	return pthread_once(&self->impl, fn);
}

#elif RBC_USE(WIN32_THREADS)

static BOOL WINAPI init_once_fn(INIT_ONCE* init_once, void* arg, void** context) {
	RbcOnceFn const fn = (RbcOnceFn) arg;
	fn();
	return TRUE;
}

RbcError rbc_call_once(RbcOnce* self, RbcOnceFn fn) {
	RBC_SYNC_CHECK_LAST_ERROR(InitOnceExecuteOnce((INIT_ONCE*) &self->impl, init_once_fn, (void*) fn, NULL));
}

#endif
