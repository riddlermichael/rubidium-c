#include "thread.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

RBC_STATIC_ASSERT(sizeof(pthread_t) <= sizeof(usize));

	#include <unistd.h>
	#ifndef RBC_COMPILER_MINGW
		#include <sched.h>
	#endif

	#if !defined(RBC_OS_ANDROID)                            \
	        && !defined(RBC_OS_OPENBSD)                     \
	        && defined(_POSIX_THREAD_PRIORITY_SCHEDULING)   \
	        && (_POSIX_THREAD_PRIORITY_SCHEDULING - 0 >= 0) \
	    || defined(RBC_OS_DARWIN)
		#define RBC_HAS_THREAD_PRIORITY_SCHEDULING 1
	#else
		#define RBC_HAS_THREAD_PRIORITY_SCHEDULING (-1)
	#endif

struct RbcThreadImpl {
	pthread_t impl;
	RbcThreadStartFn fn;
	void* arg;
	usize stack_size;
};

static _Thread_local RbcThreadImpl* this_thread_impl;

static void* start_thread(void* arg) {
	RbcThreadImpl* impl = arg;
	this_thread_impl = impl;

	isize const ret = (impl->fn)(arg); // NOLINT(*-redundant-parentheses)
	return (void*) ret; // NOLINT(*-no-int-to-ptr)
}

RbcThread rbc_thread_current(void) {
	return (RbcThread) {this_thread_impl};
}

usize rbc_thread_current_id(void) {
	return (usize) pthread_self();
}

RbcError rbc_thread_sleep_for(RbcDuration timeout) {
	RbcTimespec const ts = rbc_duration_to_timespec(timeout);
	RBC_SYNC_CHECK_LAST_ERROR(nanosleep((struct timespec const*) &ts, NULL));
}

void rbc_thread_yield(void) {
	sched_yield();
}

RbcError rbc_thread_detach(RbcThread self) {
	return pthread_detach(RBC_SYNC_DEREF_IMPL);
}

void rbc_thread_exit(int exit_code) {
	pthread_exit((void*) (isize) exit_code); // NOLINT(*-no-int-to-ptr)
}

usize rbc_thread_id(RbcThread self) {
	return self.impl ? (usize) self.impl->impl : 0;
}

RbcError rbc_thread_join_with_code(RbcThread self, int* exit_code) {
	void* ret_code = NULL;
	int const error = pthread_join(RBC_SYNC_DEREF_IMPL, &ret_code);
	if (exit_code) {
		*exit_code = (isize) ret_code; // NOLINT(*-narrowing-conversions)
	}
	return error;
}

RbcError rbc_thread_start(RbcThread self, RbcThreadStartFn fn, void* arg) {
	if (RBC_SYNC_DEREF_IMPL) {
		return RBC_ERROR_OPERATION_IN_PROGRESS;
	}

	pthread_attr_t RBC_CLEANUP(pthread_attr_destroy) attr;
	RBC_SYNC_CHECK(pthread_attr_init(&attr));
	RBC_SYNC_CHECK(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));

	if (self.impl->stack_size) {
	#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && (_POSIX_THREAD_ATTR_STACKSIZE - 0 > 0) \
	    || defined(RBC_COMPILER_MINGW)
		RBC_SYNC_CHECK(pthread_attr_setstacksize(&attr, self.impl->stack_size));
	#else
		return RBC_ERROR_FUNCTION_NOT_SUPPORTED;
	#endif
	}

	self.impl->fn = fn;
	self.impl->arg = arg;
	return pthread_create(RBC_SYNC_IMPL, &attr, start_thread, self.impl);
}

RbcError rbc_thread_wait_for_with_code(RbcThread self, RbcDuration timeout, int* exit_code) {
	RbcTime const deadline = rbc_time_deadline_from_timeout(timeout);
	return rbc_thread_wait_until_with_code(self, deadline, exit_code);
}

RbcError rbc_thread_wait_until_with_code(RbcThread self, RbcTime deadline, int* exit_code) {
	#ifdef RBC_COMPILER_MINGW

	RBC_UNUSED(self);
	RBC_UNUSED(deadline);
	RBC_UNUSED(exit_code);
	return RBC_ERROR_NOT_IMPLEMENTED;

	#elif !defined(RBC_OS_DARWIN)

	void* ret_code = NULL;
	RbcTimespec const ts = rbc_time_to_timespec(deadline);
	int const error = pthread_timedjoin_np(RBC_SYNC_DEREF_IMPL, &ret_code, (struct timespec const*) &ts);
	if (exit_code) {
		*exit_code = (isize) ret_code; // NOLINT(*-narrowing-conversions)
	}
	return error;

	#else
	RBC_UNUSED(self);
	RBC_UNUSED(deadline);
	RBC_UNUSED(exit_code);
	return RBC_ERROR_NOT_IMPLEMENTED;
	#endif
}

#elif RBC_USE(WIN32_THREADS)

	#include <process.h>

	#include <rbc/core/builtins.h>

struct RbcThreadImpl {
	HANDLE impl;
	unsigned id;
	RbcThreadStartFn fn;
	void* arg;
	usize stack_size;
};

static _Thread_local RbcThreadImpl* this_thread_impl;

static unsigned WINAPI start_thread(void* arg) {
	RbcThreadImpl* impl = arg;
	this_thread_impl = impl;
	return impl->fn(arg);
}

RbcThread rbc_thread_current(void) {
	// see https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentthread
	/**
	 * The function cannot be used by one thread to create a handle
	 * that can be used by other threads to refer to the first thread.
	 * The handle is always interpreted as referring to the thread that is using it.
	 * A thread can create a "real" handle to itself that can be used by other threads, or inherited by other processes,
	 * by specifying the pseudo handle as the source handle in a call to the DuplicateHandle function.
	 */
	// since we use `rbc_thread_equal` and `rbc_thread_cmp` which works with `id`, all should be ok
	return (RbcThread) {this_thread_impl};
}

usize rbc_thread_current_id(void) {
	return this_thread_impl ? this_thread_impl->id : 0;
}

RbcError rbc_thread_sleep_for(RbcDuration timeout) {
	if (rbc_duration_is_neg(timeout)) {
		return RBC_ERROR_INVALID_ARGUMENT;
	}

	DWORD const ms = rbc_duration_to_ms(timeout);
	Sleep(ms);
	return RBC_ERROR_OK;
}

void rbc_thread_yield(void) {
	// https://stackoverflow.com/a/1383966/4884522
	Sleep(0);
}

RbcError rbc_thread_detach(RbcThread self) {
	RBC_SYNC_CHECK_LAST_ERROR(CloseHandle(RBC_SYNC_DEREF_IMPL));
}

void rbc_thread_exit(int exit_code) {
	_endthreadex(exit_code);
	RBC_UNREACHABLE(); // suppress warning about noreturn function which returns
}

usize rbc_thread_id(RbcThread self) {
	return self.impl->id;
}

static RbcError rbc_thread_wait(RbcThread self, DWORD milliseconds, int* exit_code) {
	DWORD const code = WaitForSingleObject(RBC_SYNC_DEREF_IMPL, milliseconds);
	if (code != WAIT_OBJECT_0) {
		return rbc_error_from_last_error();
	}

	if (exit_code) {
		DWORD ret = 0;
		if (!GetExitCodeThread(RBC_SYNC_DEREF_IMPL, &ret)) {
			return rbc_error_from_last_error();
		}

		*exit_code = (int) ret;
	}
	return RBC_ERROR_OK;
}

RbcError rbc_thread_join_with_code(RbcThread self, int* exit_code) {
	return rbc_thread_wait(self, INFINITE, exit_code);
}

RbcError rbc_thread_start(RbcThread self, RbcThreadStartFn fn, void* arg) {
	if (!fn) {
		return RBC_ERROR_INVALID_ARGUMENT;
	}

	if (RBC_SYNC_DEREF_IMPL) {
		return RBC_ERROR_OPERATION_IN_PROGRESS;
	}

	self.impl->fn = fn;
	self.impl->arg = arg;
	RBC_SYNC_DEREF_IMPL = (HANDLE) _beginthreadex(
	    NULL,
	    self.impl->stack_size,
	    start_thread,
	    self.impl,
	    0,
	    &self.impl->id);
	return RBC_SYNC_DEREF_IMPL ? RBC_ERROR_OK : errno; // _beginthreadex uses errno instead of GetLastError()
}

RbcError rbc_thread_wait_for_with_code(RbcThread self, RbcDuration timeout, int* exit_code) {
	if (rbc_duration_is_neg(timeout)) {
		return RBC_ERROR_INVALID_ARGUMENT;
	}

	DWORD const ms = rbc_duration_to_ms(timeout);
	return rbc_thread_wait(self, ms, exit_code);
}

RbcError rbc_thread_wait_until_with_code(RbcThread self, RbcTime deadline, int* exit_code) {
	RbcDuration const remaining = rbc_time_sub(deadline, rbc_time_now());
	return rbc_thread_wait_for_with_code(self, remaining, exit_code);
}

#endif

// common part

RbcError rbc_thread_init(RbcThread* self) {
	self->impl = calloc(1, sizeof(RbcThreadImpl));
	return self->impl ? RBC_ERROR_OK : RBC_ERROR_NOT_ENOUGH_MEMORY;
}

RbcError rbc_thread_destroy(RbcThread* self) {
	RBC_SYNC_DESTROY(RBC_ERROR_OK);
}

int rbc_thread_cmp(RbcThread lhs, RbcThread rhs) {
	usize const lhs_id = rbc_thread_id(lhs);
	usize const rhs_id = rbc_thread_id(rhs);
	return (lhs_id > rhs_id) - (lhs_id < rhs_id);
}

bool rbc_thread_equal(RbcThread lhs, RbcThread rhs) {
	return rbc_thread_cmp(lhs, rhs) == 0;
}

RbcError rbc_thread_join(RbcThread self) {
	return rbc_thread_join_with_code(self, NULL);
}

extern void rbc_thread_quit(void);

RbcError rbc_thread_sleep_until(RbcTime deadline) {
	RbcDuration const remaining = rbc_time_sub(deadline, rbc_time_now());
	return rbc_thread_sleep_for(remaining);
}

RbcError rbc_thread_set_stack_size(RbcThread self, usize stack_size) {
	if (!self.impl) {
		return RBC_ERROR_INVALID_ARGUMENT;
	}

	if (RBC_SYNC_DEREF_IMPL) {
		return RBC_ERROR_OPERATION_IN_PROGRESS;
	}

	self.impl->stack_size = stack_size;
	return RBC_ERROR_OK;
}

usize rbc_thread_stack_size(RbcThread self) {
	return self.impl->stack_size;
}

RbcError rbc_thread_wait_for(RbcThread self, RbcDuration timeout) {
	return rbc_thread_wait_for_with_code(self, timeout, NULL);
}

RbcError rbc_thread_wait_until(RbcThread self, RbcTime deadline) {
	return rbc_thread_wait_until_with_code(self, deadline, NULL);
}
