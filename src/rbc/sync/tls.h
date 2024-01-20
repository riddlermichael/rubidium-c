#pragma once

#include <rbc/sync/common.h>

typedef struct rbc_tls_impl rbc_tls_impl;

struct rbc_tls {
	rbc_tls_impl* impl;
};
typedef struct rbc_tls rbc_tls;

RBC_BEGIN_EXTERN_C

typedef void (*rbc_tls_dtor_t)(void*);

/**
 * Creates new thread-local storage key and stores it in the object pointed to by @p self.
 * Although the same key value may be used by different threads, the values bound to the key by rbc_tls_set
 * are maintained on a per-thread basis and persist for the life of the calling thread.
 *
 * The value `NULL` is associated with the newly created key in all existing threads, and upon thread creation,
 * the values associated with all TLS keys is initialized to `NULL`.
 *
 * If @p dtor is not a null pointer, then also associates the destructor which is called when the storage is released
 * by rb_os_thread_exit (but not by rbc_tls_destroy and not at program termination by `exit`).
 *
 * A call to rbc_tls_init from within a thread-local storage destructor results in undefined behavior.
 * @param self  pointer to memory location to store the new thread-local storage key
 * @param dtor  pointer to a function to call at thread exit
 */
RBC_EXPORT rbc_error rbc_tls_init(rbc_tls* self, rbc_tls_dtor_t dtor) RBC_NONNULL_ARGS(1);

/**
 * Destroys the thread-local storage identified by @p self.
 *
 * The destructor, if one was registered by rbc_tls_init, is not called
 * (they are only called at thread exit, either by rb_os_thread_exit or by returning from the thread function),
 * it is the responsibility of the programmer to ensure that every thread that is aware of @p self
 * performed all necessary cleanup, before the call to rbc_tls_destroy is made.
 *
 * If rbc_tls_destroy is called while another thread is executing destructors for @p self,
 * it's unspecified whether this changes the number of invocations to the associated destructor.
 *
 * If rbc_tls_destroy is called while the calling thread is executing destructors,
 * then the destructor associated with @p self will not be executed again on this thread.
 */
RBC_EXPORT rbc_error rbc_tls_destroy(rbc_tls* self) RBC_NONNULL;

/**
 * Returns the value held in thread-local storage for the current thread identified by @p self.
 * Different threads may get different values identified by the same key.
 *
 * On thread startup (see rb_os_thread_create), the values associated with all TLS keys are `NULL`.
 * Different value may be placed in the thread-local storage with rbc_tls_set.
 */
RBC_EXPORT rbc_error rbc_tls_get(rbc_tls self, void** value) RBC_NONNULL;

/**
 * Sets the value of the thread-local storage identified by @p self for the current thread to @p value.
 * Different threads may set different values to the same key.
 *
 * The destructor, if available, is not invoked.
 */
RBC_EXPORT rbc_error rbc_tls_set(rbc_tls self, void const* value) RBC_NONNULL;

RBC_END_EXTERN_C
