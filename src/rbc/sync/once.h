#pragma once

#include <rbc/sync/common.h>

#if RBC_USE(PTHREADS)

	#include <pthread.h>

struct RbcOnce {
	pthread_once_t impl;
};

	#define RBC_ONCE_INIT     \
		(RbcOnce) {           \
			PTHREAD_ONCE_INIT \
		}

#elif RBC_USE(WIN32_THREADS)

struct RbcOnce {
	void* impl;
};

	#define RBC_ONCE_INIT \
		{                 \
			{             \
				NULL      \
			}             \
		}

#endif

typedef struct RbcOnce RbcOnce;

RBC_BEGIN_EXTERN_C

typedef void (*RbcOnceFn)(void);

RBC_EXPORT RbcError rbc_call_once(RbcOnce* self, RbcOnceFn fn) RBC_NONNULL;

RBC_END_EXTERN_C
