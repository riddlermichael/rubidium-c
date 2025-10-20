#pragma once

#include <string.h>

#include <rbc/core/attributes.h>
#include <rbc/core/endian.h>
#include <rbc/core/keywords.h>
#include <rbc/core/types.h>

#if defined(RBC_COMPILER_GCC_LIKE)

static inline RBC_CONST RBC_NOTHROW u64 rbc_bswap_u64(u64 value) {
	return __builtin_bswap64(value);
}

static inline RBC_CONST RBC_NOTHROW u32 rbc_bswap_u32(u32 value) {
	return __builtin_bswap32(value);
}

static inline RBC_CONST RBC_NOTHROW u16 rbc_bswap_u16(u16 value) {
	return __builtin_bswap16(value);
}

#else

// These definitions are written so that they are recognized by most compilers
// as `bswap` and replaced with single instruction builtins if available

static inline RBC_CONST RBC_NOTHROW u64 rbc_bswap_u64(u64 value) {
	return 0
	    | ((value & 0x00000000000000ffULL) << 56)
	    | ((value & 0x000000000000ff00ULL) << 40)
	    | ((value & 0x0000000000ff0000ULL) << 24)
	    | ((value & 0x00000000ff000000ULL) << 8)
	    | ((value & 0x000000ff00000000ULL) >> 8)
	    | ((value & 0x0000ff0000000000ULL) >> 24)
	    | ((value & 0x00ff000000000000ULL) >> 40)
	    | ((value & 0xff00000000000000ULL) >> 56);
}

static inline RBC_CONST RBC_NOTHROW u32 rbc_bswap_u32(u32 value) {
	return 0
	    | ((value & 0x000000ffU) << 24)
	    | ((value & 0x0000ff00U) << 8)
	    | ((value & 0x00ff0000U) >> 8)
	    | ((value & 0xff000000U) >> 24);
}

static inline RBC_CONST RBC_NOTHROW u16 rbc_bswap_u16(u16 value) {
	return 0
	    | ((value & 0x00ffU) << 8)
	    | ((value & 0xff00U) >> 8);
}

#endif

inline static RBC_CONST RBC_NOTHROW u8 rbc_bswap_u8(u8 value) {
	return value;
}

// signed specializations

static inline RBC_CONST RBC_NOTHROW i64 rbc_bswap_i64(i64 value) {
	return (i64) rbc_bswap_u64((u64) value);
}

static inline RBC_CONST RBC_NOTHROW i32 rbc_bswap_i32(i32 value) {
	return (i32) rbc_bswap_u64((u32) value);
}

static inline RBC_CONST RBC_NOTHROW i16 rbc_bswap_i16(i16 value) {
	return (i16) rbc_bswap_u64((u16) value);
}

static inline RBC_CONST RBC_NOTHROW i8 rbc_bswap_i8(i8 value) {
	return value;
}

// floating specializations

static inline RBC_CONST RBC_NOTHROW f32 rbc_bswap_f32(f32 value) {
	u32 x = 0;
	memcpy(&x, &value, sizeof(x));
	x = rbc_bswap_u32(x);
	memcpy(&value, &x, sizeof(x));
	return value;
}

static inline RBC_CONST RBC_NOTHROW f64 rbc_bswap_f64(f64 value) {
	u64 x = 0;
	memcpy(&x, &value, sizeof(x));
	x = rbc_bswap_u64(x);
	memcpy(&value, &x, sizeof(x));
	return value;
}

// NOLINTBEGIN(readability-identifier-naming)

#ifndef __cplusplus

	#define rbc_bswap(value)    \
		_Generic((value),       \
		    i8: rbc_bswap_i8,   \
		    i16: rbc_bswap_i16, \
		    i32: rbc_bswap_i32, \
		    i64: rbc_bswap_i64, \
		    u8: rbc_bswap_u8,   \
		    u16: rbc_bswap_u16, \
		    u32: rbc_bswap_u32, \
		    u64: rbc_bswap_u64, \
		    f32: rbc_bswap_f32, \
		    f64: rbc_bswap_f64)(value)

#endif

#if RBC_IS_LITTLE_ENDIAN

	#define rbc_to_le(value) (value)
	#define rbc_from_le(value) (value)
	#define rbc_to_be(value) rbc_bswap(value)
	#define rbc_from_be(value) rbc_bswap(value)

#elif RBC_IS_BIG_ENDIAN

	#define rbc_to_le(value) rbc_bswap(value)
	#define rbc_from_le(value) rbc_bswap(value)
	#define rbc_to_be(value) (value)
	#define rbc_from_be(value) (value)

#endif

// NOLINTEND(readability-identifier-naming)
