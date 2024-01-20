#pragma once

#include <rbc/core/processor.h>

// Machine byte-order, reuse preprocessor provided macros when available
#if defined(__ORDER_BIG_ENDIAN__)
	#define RBC_BIG_ENDIAN __ORDER_BIG_ENDIAN__
#else
	#define RBC_BIG_ENDIAN 4321
#endif
#if defined(__ORDER_LITTLE_ENDIAN__)
	#define RBC_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#else
	#define RBC_LITTLE_ENDIAN 1234
#endif

#if defined(RBC_PROCESSOR_ARM)
	#if defined(__ARMEL__) || defined(_M_ARM64)
		#define RBC_BYTE_ORDER RBC_LITTLE_ENDIAN
	#elif defined(__ARMEB__)
		#define RBC_BYTE_ORDER RBC_BIG_ENDIAN
	#endif
#elif defined(RBC_PROCESSOR_MIPS)
	#if defined(__MIPSEL__)
		#define RBC_BYTE_ORDER RBC_LITTLE_ENDIAN
	#elif defined(__MIPSEB__)
		#define RBC_BYTE_ORDER RBC_BIG_ENDIAN
	#endif
#elif defined(RBC_PROCESSOR_X86) || defined(RBC_PROCESSOR_RISCV) || defined(RBC_PROCESSOR_WASM)
	#define RBC_BYTE_ORDER RBC_LITTLE_ENDIAN
#elif defined(RBC_PROCESSOR_S390) || defined(RBC_PROCESSOR_SPARC)
	#define RBC_BYTE_ORDER RBC_BIG_ENDIAN
#endif

// Some processors support either endian format, try to detect which we are using
#ifndef RBC_BYTE_ORDER
	#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == RBC_BIG_ENDIAN || __BYTE_ORDER__ == RBC_LITTLE_ENDIAN)
	    // Reuse __BYTE_ORDER__ as-is, since our RBC_*_ENDIAN #defines match the preprocessor defaults
		#define RBC_BYTE_ORDER __BYTE_ORDER__
	#elif defined(__BIG_ENDIAN__) || defined(_big_endian__) || defined(_BIG_ENDIAN)
		#define RBC_BYTE_ORDER RBC_BIG_ENDIAN
	#elif defined(__LITTLE_ENDIAN__) || defined(_little_endian__) || defined(_LITTLE_ENDIAN) || defined(WINAPI_FAMILY)
		#define RBC_BYTE_ORDER RBC_LITTLE_ENDIAN
	#else
		#error "Unable to determine byte order"
	#endif
#endif

enum rbc_endian {
	RBC_ENDIAN_LITTLE = RBC_LITTLE_ENDIAN,
	RBC_ENDIAN_BIG = RBC_BIG_ENDIAN,
	RBC_ENDIAN_NATIVE = RBC_BYTE_ORDER
};

#define RBC_IS_LITTLE_ENDIAN (RBC_BYTE_ORDER == RBC_LITTLE_ENDIAN)
#define RBC_IS_BIG_ENDIAN (RBC_BYTE_ORDER == RBC_BIG_ENDIAN)
// very exotic case
#define RBC_IS_MIXED_ENDIAN (RBC_BYTE_ORDER != RBC_BIG_ENDIAN && RBC_BYTE_ORDER != RBC_LITTLE_ENDIAN)
