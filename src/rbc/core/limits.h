#pragma once

#include <rbc/core/processor.h>

#define RBC_I8_MIN (-127 - 1)
#define RBC_I8_MAX 127
#define RBC_I16_MIN (-32767 - 1)
#define RBC_I16_MAX 32767
#define RBC_I32_MIN (-2147483647 - 1)
#define RBC_I32_MAX 2147483647
#define RBC_I64_MAX 9223372036854775807LL
#define RBC_I64_MIN (-RBC_I64_MAX - 1LL)

#define RBC_U8_MIN 0U
#define RBC_U8_MAX 255U
#define RBC_U16_MIN 0U
#define RBC_U16_MAX 65535U
#define RBC_U32_MIN 0U
#define RBC_U32_MAX 4294967295U
#define RBC_U64_MIN 0U
#define RBC_U64_MAX 18446744073709551615ULL

#if RBC_IS_64BIT
	#define RBC_ISIZE_MIN RBC_I64_MIN
	#define RBC_ISIZE_MAX RBC_I64_MAX
	#define RBC_USIZE_MIN RBC_U64_MIN
	#define RBC_USIZE_MAX RBC_U64_MAX
#elif RBC_IS_32BIT
	#define RBC_ISIZE_MIN RBC_I32_MIN
	#define RBC_ISIZE_MAX RBC_I32_MAX
	#define RBC_USIZE_MIN RBC_U32_MIN
	#define RBC_USIZE_MAX RBC_U32_MAX
#endif

#define RBC_F32_MIN 0x1p-126F
#define RBC_F32_MAX 0x1.fffffep127F
#define RBC_F32_INF __builtin_huge_valf()
#define RBC_F64_MIN 0x1p-1022
#define RBC_F64_MAX 0x1.fffffffffffffp1023
#define RBC_F64_INF __builtin_huge_val()

#define RBC_MIN(T) _Generic((T) 0, \
	i8: RBC_I8_MIN,                \
	i16: RBC_I16_MIN,              \
	i32: RBC_I32_MIN,              \
	i64: RBC_I64_MIN,              \
	u8: RBC_U8_MIN,                \
	u16: RBC_U16_MIN,              \
	u32: RBC_U32_MIN,              \
	u64: RBC_U64_MIN,              \
	f32: RBC_F32_MIN,              \
	f64: RBC_F64_MIN)

#define RBC_MAX(T) _Generic((T) 0, \
	i8: RBC_I8_MAX,                \
	i16: RBC_I16_MAX,              \
	i32: RBC_I32_MAX,              \
	i64: RBC_I64_MAX,              \
	u8: RBC_U8_MAX,                \
	u16: RBC_U16_MAX,              \
	u32: RBC_U32_MAX,              \
	u64: RBC_U64_MAX,              \
	f32: RBC_F32_MAX,              \
	f64: RBC_F64_MAX)
