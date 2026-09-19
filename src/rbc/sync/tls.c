#include "tls.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

struct RbcTlsImpl {
	pthread_key_t impl;
};

RbcError rbc_tls_init(RbcTls* self, RbcTlsDtor dtor) {
	RBC_SYNC_INIT(RbcTls);
	RBC_SYNC_CHECK_INIT(pthread_key_create(RBC_SYNC_IMPL_PTR, dtor));
}

RbcError rbc_tls_destroy(RbcTls* self) {
	RBC_SYNC_DESTROY(pthread_key_delete(*RBC_SYNC_IMPL_PTR));
}

RbcError rbc_tls_get(RbcTls self, void** value) {
	// No errors are returned from pthread_getspecific().
	// If no thread-specific data value is associated with a key, then the value NULL shall be returned.
	*value = pthread_getspecific(*RBC_SYNC_IMPL);
	return RBC_ERROR_OK;
}

RbcError rbc_tls_set(RbcTls self, void const* value) {
	return pthread_setspecific(*RBC_SYNC_IMPL, value);
}

#elif RBC_USE(WIN32_THREADS)

struct RbcTlsImpl {
	DWORD impl;
};

RbcError rbc_tls_init(RbcTls* self, RbcTlsDtor dtor) {
	RBC_UNUSED(dtor);
	RBC_SYNC_INIT(RbcTls);
	RBC_SYNC_DEREF_IMPL_PTR = TlsAlloc();
	return RBC_SYNC_DEREF_IMPL_PTR == TLS_OUT_OF_INDEXES
	    ? rbc_error_from_last_error()
	    : RBC_ERROR_OK;
}

RbcError rbc_tls_destroy(RbcTls* self) {
	RBC_SYNC_DESTROY(TlsFree(RBC_SYNC_DEREF_IMPL_PTR));
}

RbcError rbc_tls_get(RbcTls self, void** value) {
	*value = TlsGetValue(RBC_SYNC_DEREF_IMPL);
	return *value ? RBC_ERROR_OK : rbc_error_from_last_error();
}

RbcError rbc_tls_set(RbcTls self, void const* value) {
	RBC_SYNC_CHECK_LAST_ERROR(TlsSetValue(RBC_SYNC_DEREF_IMPL, (void*) value));
}

#endif
