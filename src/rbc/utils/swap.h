#pragma once

#include <rbc/core/compiler.h>

#ifdef RBC_COMPILER_GCC_LIKE

	#include <rbc/core/keywords.h>
	#include <rbc/core/macros.h>

	#define RBC_SWAP(lhs, rhs)                                           \
		do {                                                             \
			RBC_STATIC_ASSERT_X(RBC_TYPES_COMPATIBLE_EXPR((lhs), (rhs)), \
			    "Types of lhs and rhs are incompatible");                \
			__typeof__(lhs) t = lhs;                                     \
			lhs = rhs;                                                   \
			rhs = t;                                                     \
		} while (0)

#else

	#include <string.h>
	#ifdef RBC_COMPILER_MSVC
		#include <stdlib.h>
	#endif

	#include <rbc/core/types.h>

inline static void rbc_swap(void* lhs, void* rhs, usize size) {
	if (lhs == rhs) {
		return;
	}

	#ifdef RBC_COMPILER_MSVC

	char* storage = (char*) malloc(size);
	if (storage) {
		memcpy(storage, lhs, size);
		memmove(lhs, rhs, size);
		memcpy(rhs, storage, size);
	} else {
		#define RBC_SWAP_BYTE(x, y) \
			do {                    \
				u8 t = x;           \
				x = y;              \
				y = t;              \
			} while (0)

		u8* const lhs_u8 = (u8*) lhs;
		u8* const rhs_u8 = (u8*) rhs;
		for (usize idx = 0; idx < size; ++idx) {
			RBC_SWAP_BYTE(lhs_u8[idx], rhs_u8[idx]);
		}

		#undef RBC_SWAP_BYTE
	}
	free(storage);

	#else

	char storage[size];
	memcpy(storage, lhs, size);
	memmove(lhs, rhs, size);
	memcpy(rhs, storage, size);

	#endif
}

/* TODO improve for primitive types using _Generic */

	#define RBC_SWAP(lhs, rhs)                              \
		do {                                                \
			RBC_STATIC_ASSERT_X(sizeof(lhs) == sizeof(rhs), \
			    "lhs and rhs have different sizes");        \
			rbc_swap(&(lhs), &(rhs), sizeof(lhs));          \
		} while (0)

#endif
