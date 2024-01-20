#include "once.h"

#if RBC_USE(PTHREADS)

rbc_error rbc_call_once(rbc_once* once, rbc_once_fn fn) {
	return pthread_once(&once->impl, fn);
}

#endif
