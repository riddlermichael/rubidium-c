#pragma once

#include <rbc/sync/common.h>
#include <rbc/time/time.h>

typedef struct rbc_thread_impl rbc_thread_impl;

struct rbc_thread {
	rbc_thread_impl* impl;
};
typedef struct rbc_thread rbc_thread;

RBC_BEGIN_EXTERN_C

typedef int (*rbc_thread_start_fn)(void*);

// static

RBC_EXPORT rbc_thread rbc_thread_current(void);

RBC_EXPORT usize rbc_thread_current_id(void);

RBC_EXPORT rbc_error rbc_thread_sleep_for(rbc_duration timeout);

RBC_EXPORT rbc_error rbc_thread_sleep_until(rbc_time deadline);

RBC_EXPORT void rbc_thread_yield(void);

// instance

RBC_EXPORT rbc_error rbc_thread_init(rbc_thread* self) RBC_NONNULL;

RBC_EXPORT rbc_error rbc_thread_destroy(rbc_thread* self) RBC_NONNULL;

RBC_EXPORT int rbc_thread_cmp(rbc_thread lhs, rbc_thread rhs);

RBC_EXPORT rbc_error rbc_thread_detach(rbc_thread self);

RBC_EXPORT bool rbc_thread_equal(rbc_thread lhs, rbc_thread rhs);

#ifndef RBC_COMPILER_MINGW
RBC_NORETURN
#endif
RBC_EXPORT void rbc_thread_exit(int exit_code);

RBC_EXPORT usize rbc_thread_id(rbc_thread self);

RBC_EXPORT rbc_error rbc_thread_join(rbc_thread self);

RBC_EXPORT rbc_error rbc_thread_join_with_code(rbc_thread self, int* exit_code);

#ifndef RBC_COMPILER_MINGW
RBC_NORETURN
#endif
/// Equivalent to calling `rbc_thread_exit(0)`.
inline static RBC_EXPORT void rbc_thread_quit(void) {
	rbc_thread_exit(0);
}

RBC_EXPORT rbc_error rbc_thread_set_stack_size(rbc_thread self, usize stack_size);

RBC_EXPORT usize rbc_thread_stack_size(rbc_thread self);

RBC_EXPORT rbc_error rbc_thread_start(rbc_thread self, rbc_thread_start_fn fn, void* arg) RBC_NONNULL_ARGS(2);

RBC_EXPORT rbc_error rbc_thread_wait_for(rbc_thread self, rbc_duration timeout);

RBC_EXPORT rbc_error rbc_thread_wait_for_with_code(rbc_thread self, rbc_duration timeout, int* exit_code);

RBC_EXPORT rbc_error rbc_thread_wait_until(rbc_thread self, rbc_time deadline);

RBC_EXPORT rbc_error rbc_thread_wait_until_with_code(rbc_thread self, rbc_time deadline, int* exit_code);

RBC_END_EXTERN_C
