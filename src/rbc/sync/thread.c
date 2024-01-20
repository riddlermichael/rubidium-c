#include "thread.h"

#include <rbc/sync/impl.h>

#if RBC_USE(PTHREADS)

RBC_STATIC_ASSERT(sizeof(pthread_t) <= sizeof(usize));

	#include <unistd.h>
	#ifndef RB_COMPILER_MINGW
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

struct rbc_thread_impl {
	pthread_t impl;
	rbc_thread_start_fn fn;
	void* arg;
	usize stack_size;
};

static _Thread_local rbc_thread_impl* this_thread_impl;

static void* start_thread(void* arg) {
	rbc_thread_impl* impl = (rbc_thread_impl*) arg;
	this_thread_impl = impl;

	isize const ret = (impl->fn)(arg);
	return (void*) ret; // NOLINT(*-no-int-to-ptr)
}

rbc_thread rbc_thread_current(void) {
	return (rbc_thread){this_thread_impl};
}

usize rbc_thread_current_id(void) {
	return (usize) pthread_self();
}

rbc_error rbc_thread_sleep_for(rbc_duration timeout) {
	rbc_timespec const ts = rbc_duration_to_timespec(timeout);
	RBC_SYNC_CHECK_ERRNO(nanosleep((struct timespec const*) &ts, NULL));
}

rbc_error rbc_thread_sleep_until(rbc_time deadline) {
	rbc_duration const remaining = rbc_time_sub(deadline, rbc_time_now());
	return rbc_thread_sleep_for(remaining);
}

void rbc_thread_yield(void) {
	sched_yield();
}

rbc_error rbc_thread_init(rbc_thread* self) {
	self->impl = calloc(1, sizeof(rbc_thread_impl));
	return self->impl ? RBC_ERROR_OK : RBC_ERROR_NOT_ENOUGH_MEMORY;
}

rbc_error rbc_thread_destroy(rbc_thread* self) {
	RBC_SYNC_DESTROY(RBC_ERROR_OK);
}

int rbc_thread_cmp(rbc_thread lhs, rbc_thread rhs) {
	usize const lhs_id = rbc_thread_id(lhs);
	usize const rhs_id = rbc_thread_id(rhs);
	return (lhs_id > rhs_id) - (lhs_id < rhs_id);
}

rbc_error rbc_thread_detach(rbc_thread self) {
	return pthread_detach(*RBC_SYNC_IMPL);
}

bool rbc_thread_equal(rbc_thread lhs, rbc_thread rhs) {
	return rbc_thread_cmp(lhs, rhs) == 0;
}

void rbc_thread_exit(int exit_code) {
	pthread_exit((void*) (isize) exit_code); // NOLINT(*-no-int-to-ptr)
}

usize rbc_thread_id(rbc_thread self) {
	return self.impl ? (usize) self.impl->impl : 0;
}

rbc_error rbc_thread_join(rbc_thread self) {
	return rbc_thread_join_with_code(self, NULL);
}

rbc_error rbc_thread_join_with_code(rbc_thread self, int* exit_code) {
	void* ret_code = NULL;
	int const error = pthread_join(*RBC_SYNC_IMPL, &ret_code);
	if (exit_code) {
		*exit_code = (isize) ret_code; // NOLINT(*-narrowing-conversions)
	}
	return error;
}

extern void rbc_thread_quit(void);

rbc_error rbc_thread_set_stack_size(rbc_thread self, usize stack_size) {
	if (!self.impl) {
		return RBC_ERROR_INVALID_ARGUMENT;
	}

	if (*RBC_SYNC_IMPL) {
		return RBC_ERROR_OPERATION_IN_PROGRESS;
	}

	self.impl->stack_size = stack_size;
	return RBC_ERROR_OK;
}

usize rbc_thread_stack_size(rbc_thread self) {
	return self.impl->stack_size;
}

rbc_error rbc_thread_start(rbc_thread self, rbc_thread_start_fn fn, void* arg) {
	if (*RBC_SYNC_IMPL) {
		return RBC_ERROR_OPERATION_IN_PROGRESS;
	}

	pthread_attr_t RBC_CLEANUP(pthread_attr_destroy) attr;
	RBC_SYNC_CHECK(pthread_attr_init(&attr));
	RBC_SYNC_CHECK(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));

	if (self.impl->stack_size) {
	#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && (_POSIX_THREAD_ATTR_STACKSIZE - 0 > 0)
		RBC_SYNC_CHECK(pthread_attr_setstacksize(&attr, self.impl->stack_size));
	#else
		return RBC_ERROR_FUNCTION_NOT_SUPPORTED;
	#endif
	}

	self.impl->fn = fn;
	self.impl->arg = arg;
	return pthread_create(RBC_SYNC_IMPL, &attr, start_thread, self.impl);
}

rbc_error rbc_thread_wait_for(rbc_thread self, rbc_duration timeout) {
	return rbc_thread_wait_for_with_code(self, timeout, NULL);
}

rbc_error rbc_thread_wait_for_with_code(rbc_thread self, rbc_duration timeout, int* exit_code) {
	rbc_time deadline = rbc_time_deadline_from_timeout(timeout);
	return rbc_thread_wait_until_with_code(self, deadline, exit_code);
}

rbc_error rbc_thread_wait_until(rbc_thread self, rbc_time deadline) {
	return rbc_thread_wait_until_with_code(self, deadline, NULL);
}

rbc_error rbc_thread_wait_until_with_code(rbc_thread self, rbc_time deadline, int* exit_code) {
	void* ret_code = NULL;
	rbc_timespec ts = rbc_time_to_timespec(deadline);
	int const error = pthread_timedjoin_np(*RBC_SYNC_IMPL, &ret_code, (struct timespec const*) &ts);
	if (exit_code) {
		*exit_code = (isize) ret_code; // NOLINT(*-narrowing-conversions)
	}
	return error;
}

#endif
