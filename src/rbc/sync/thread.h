#pragma once

#include <rbc/sync/common.h>
#include <rbc/time/time.h>

typedef struct RbcThreadImpl RbcThreadImpl;

struct RbcThread {
	RbcThreadImpl* impl;
};
typedef struct RbcThread RbcThread;

RBC_BEGIN_EXTERN_C

typedef int (*RbcThreadStartFn)(void*);

// static

RBC_EXPORT RbcThread rbc_thread_current(void);

RBC_EXPORT usize rbc_thread_current_id(void);

RBC_EXPORT RbcError rbc_thread_sleep_for(RbcDuration timeout);

RBC_EXPORT RbcError rbc_thread_sleep_until(RbcTime deadline);

RBC_EXPORT void rbc_thread_yield(void);

// instance

RBC_EXPORT RbcError rbc_thread_init(RbcThread* self) RBC_NONNULL;

RBC_EXPORT RbcError rbc_thread_destroy(RbcThread* self) RBC_NONNULL;

RBC_EXPORT int rbc_thread_cmp(RbcThread lhs, RbcThread rhs);

RBC_EXPORT RbcError rbc_thread_detach(RbcThread self);

RBC_EXPORT bool rbc_thread_equal(RbcThread lhs, RbcThread rhs);

#ifndef RBC_COMPILER_MINGW
RBC_NORETURN
#endif
RBC_EXPORT void rbc_thread_exit(int exit_code);

RBC_EXPORT usize rbc_thread_id(RbcThread self);

RBC_EXPORT RbcError rbc_thread_join(RbcThread self);

RBC_EXPORT RbcError rbc_thread_join_with_code(RbcThread self, int* exit_code);

#ifndef RBC_COMPILER_MINGW
RBC_NORETURN
#endif
/// Equivalent to calling `rbc_thread_exit(0)`.
static inline RBC_EXPORT void rbc_thread_quit(void) {
	rbc_thread_exit(0);
}

RBC_EXPORT RbcError rbc_thread_set_stack_size(RbcThread self, usize stack_size);

RBC_EXPORT usize rbc_thread_stack_size(RbcThread self);

RBC_EXPORT RbcError rbc_thread_start(RbcThread self, RbcThreadStartFn fn, void* arg) RBC_NONNULL_ARGS(2);

RBC_EXPORT RbcError rbc_thread_wait_for(RbcThread self, RbcDuration timeout);

RBC_EXPORT RbcError rbc_thread_wait_for_with_code(RbcThread self, RbcDuration timeout, int* exit_code);

RBC_EXPORT RbcError rbc_thread_wait_until(RbcThread self, RbcTime deadline);

RBC_EXPORT RbcError rbc_thread_wait_until_with_code(RbcThread self, RbcTime deadline, int* exit_code);

RBC_END_EXTERN_C
