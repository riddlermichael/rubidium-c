#include "assert.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <rbc/core/compiler.h>

#if defined(RBC_COMPILER_MSVC)
	#define RBC_ASSERT_HANDLER_NEED_MUTEX 1
#else
	#include <stdatomic.h>

	#define RBC_ASSERT_HANDLER_NEED_MUTEX (ATOMIC_POINTER_LOCK_FREE < 2)
#endif

#if RBC_ASSERT_HANDLER_NEED_MUTEX

	#include <rb/sync/mutex.h>
	#include <rb/sync/once.h>

static rbc_mutex* g_assert_handler_mutex;
static rbc_once_flag g_init_mutex_once = RBC_ONCE_FLAG_INIT;

static void rbc_init_assert_handler_mutex(void) {
	rbc_mutex_init(&g_assert_handler_mutex);
}

static rbc_assert_handler g_assert_handler RBC_PT_GUARDED_BY(g_assert_handler_mutex);

#else

static rbc_assert_handler _Atomic g_assert_handler;

#endif

void rbc_throw_assert(char const* msg, rbc_source_location loc) {
	rbc_throw_assert_x(msg, loc, NULL);
}

void rbc_throw_assert_x(char const* msg, rbc_source_location loc, char const* fmt, ...) {
	(void) fprintf(stderr, "Assertion failed: ");
	(void) fprintf(stderr, "%s", msg);

	if (fmt) {
		(void) fprintf(stderr, " (");
		va_list varargs; // NOLINT(cppcoreguidelines-init-variables)
		va_start(varargs, fmt);
		(void) vfprintf(stderr, fmt, varargs);
		va_end(varargs);
		(void) fprintf(stderr, ")");
	}

	(void) fprintf(stderr, " at ");
	rbc_source_location_print_to(loc, stderr);
	(void) fprintf(stderr, "\n");
	(void) fflush(stderr);
	rbc_assert_handler handler = rbc_get_assert_handler();
	if (handler) {
		handler(msg, loc);
	}
	exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
}

#if RBC_ASSERT_HANDLER_NEED_MUTEX

rbc_assert_handler rbc_get_assert_handler(void) {
	rbc_call_once(&g_init_mutex_once, rbc_init_assert_handler_mutex);
	rbc_mutex_lock(g_assert_handler_mutex);
	rbc_assert_handler handler = g_assert_handler;
	rbc_mutex_unlock(g_assert_handler_mutex);
	return handler;
}

rbc_assert_handler rbc_set_assert_handler(rbc_assert_handler handler) {
	rbc_call_once(&g_init_mutex_once, rbc_init_assert_handler_mutex);
	rbc_mutex_lock(g_assert_handler_mutex);
	rbc_assert_handler prev_handler = g_assert_handler;
	g_assert_handler = handler;
	rbc_mutex_unlock(g_assert_handler_mutex);
	return prev_handler;
}

#else

rbc_assert_handler rbc_get_assert_handler(void) {
	return atomic_load_explicit(&g_assert_handler, memory_order_acquire);
}

rbc_assert_handler rbc_set_assert_handler(rbc_assert_handler handler) {
	return atomic_exchange_explicit(&g_assert_handler, handler, memory_order_acq_rel);
}

#endif
