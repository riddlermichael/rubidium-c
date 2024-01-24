#pragma once

#include <stdlib.h>

#if RBC_USE(PTHREADS)
	#include <pthread.h>
#elif RBC_USE(WIN32_THREADS)
	#include <Windows.h>
#endif

#include <rbc/core/error.h>
#include <rbc/core/os.h>

#define RBC_SYNC_GET_IMPL(self) &(self).impl->impl

#define RBC_SYNC_IMPL &self.impl->impl

#define RBC_SYNC_IMPL_PTR &self->impl->impl

#define RBC_SYNC_DEREF_IMPL self.impl->impl

#define RBC_SYNC_DEREF_IMPL_PTR self->impl->impl

#define RBC_SYNC_INIT(T)                        \
	do {                                        \
		self->impl = malloc(sizeof(T##_impl));  \
		if (!self->impl) {                      \
			return RBC_ERROR_NOT_ENOUGH_MEMORY; \
		}                                       \
	} while (0)

#define RBC_SYNC_CHECK(expr)   \
	do {                       \
		int const _err = expr; \
		if (_err) {            \
			return _err;       \
		}                      \
	} while (0)

#ifdef RBC_OS_WIN
	#define RBC_SYNC_CHECK_LAST_ERROR(expr)         \
		do {                                        \
			unsigned long const _err = expr;        \
			return _err                             \
			         ? RBC_ERROR_OK                 \
			         : rbc_error_from_last_error(); \
		} while (0)
#else
	#define RBC_SYNC_CHECK_LAST_ERROR(expr) \
		do {                                \
			int const _err = expr;          \
			return _err ? errno : 0;        \
		} while (0)
#endif

#define RBC_SYNC_CHECK_INIT(expr) \
	do {                          \
		int const _err = expr;    \
		if (_err) {               \
			free(self->impl);     \
			self->impl = NULL;    \
			return _err;          \
		}                         \
	} while (0);                  \
	return RBC_ERROR_OK

#define RBC_SYNC_DESTROY(dtor) \
	if (!self->impl) {         \
		return RBC_ERROR_OK;   \
	}                          \
                               \
	int const _err = dtor;     \
	free(self->impl);          \
	self->impl = NULL;         \
	return _err
