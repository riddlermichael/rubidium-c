#pragma once

#include <rbc/sync/common.h>

#if RBC_USE(PTHREADS)

	#include <pthread.h>

struct rbc_once {
	pthread_once_t impl;
};

	#define RBC_ONCE_INIT     \
		(rbc_once) {          \
			PTHREAD_ONCE_INIT \
		}

#elif RBC_USE(WIN32_THREADS)

	#include <Windows.h>

struct rbc_once {
	INIT_ONCE impl;
};

	#define RBC_ONCE_INIT         \
		(rbc_once) {              \
			INIT_ONCE_STATIC_INIT \
		}

#endif

typedef struct rbc_once rbc_once;

RBC_BEGIN_EXTERN_C

typedef void (*rbc_once_fn)(void);

RBC_EXPORT rbc_error rbc_call_once(rbc_once* once, rbc_once_fn fn) RBC_NONNULL;

RBC_END_EXTERN_C
