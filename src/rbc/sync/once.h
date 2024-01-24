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

struct rbc_once {
	void* impl;
};

	#define RBC_ONCE_INIT \
		{                 \
			{ NULL }      \
		}

#endif

typedef struct rbc_once rbc_once;

RBC_BEGIN_EXTERN_C

typedef void (*rbc_once_fn)(void);

RBC_EXPORT rbc_error rbc_call_once(rbc_once* self, rbc_once_fn fn) RBC_NONNULL;

RBC_END_EXTERN_C
