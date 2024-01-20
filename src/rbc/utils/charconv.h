#pragma once

#include <rbc/core/attributes.h>
#include <rbc/core/error.h>
#include <rbc/core/export.h>
#include <rbc/core/keywords.h>
#include <rbc/core/types.h>

enum {
	/// Maximal length of a result string (not including '\0') produced by rbc_to_chars
	/// (i.e. length of `"18446744073709551615"` or `"-9223372036854775808"`).
	RBC_TO_CHARS_RESULT_MAX_LEN = 20
};

struct rbc_from_chars_result {
	char const* ptr;
	rbc_error error;
};

typedef struct rbc_from_chars_result rbc_from_chars_result;

struct rbc_to_chars_result {
	char* ptr;
	rbc_error error;
};

typedef struct rbc_to_chars_result rbc_to_chars_result;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_i8(char const* first, char const* last, i8* value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_i16(char const* first, char const* last, i16* value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_i32(char const* first, char const* last, i32* value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_i64(char const* first, char const* last, i64* value);

RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_u8(char const* first, char const* last, u8* value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_u16(char const* first, char const* last, u16* value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_u32(char const* first, char const* last, u32* value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_from_chars_result rbc_from_chars_u64(char const* first, char const* last, u64* value);

RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_i8(char* first, char* last, i8 value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_i16(char* first, char* last, i16 value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_i32(char* first, char* last, i32 value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_i64(char* first, char* last, i64 value);

RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_u8(char* first, char* last, u8 value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_u16(char* first, char* last, u16 value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_u32(char* first, char* last, u32 value);
RBC_EXPORT RBC_NOTHROW RBC_PURE rbc_to_chars_result rbc_to_chars_u64(char* first, char* last, u64 value);

RBC_END_EXTERN_C

#ifdef __cplusplus

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, i8* value) noexcept {
	return rbc_from_chars_i8(first, last, value);
}

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, i16* value) noexcept {
	return rbc_from_chars_i16(first, last, value);
}

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, i32* value) noexcept {
	return rbc_from_chars_i32(first, last, value);
}

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, i64* value) noexcept {
	return rbc_from_chars_i64(first, last, value);
}

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, u8* value) noexcept {
	return rbc_from_chars_u8(first, last, value);
}

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, u16* value) noexcept {
	return rbc_from_chars_u16(first, last, value);
}

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, u32* value) noexcept {
	return rbc_from_chars_u32(first, last, value);
}

rbc_from_chars_result rbc_from_chars(char const* first, char const* last, u64* value) noexcept {
	return rbc_from_chars_u64(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, i8 value) noexcept {
	return rbc_to_chars_i8(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, i16 value) noexcept {
	return rbc_to_chars_i16(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, i32 value) noexcept {
	return rbc_to_chars_i32(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, i64 value) noexcept {
	return rbc_to_chars_i64(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, u8 value) noexcept {
	return rbc_to_chars_u8(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, u16 value) noexcept {
	return rbc_to_chars_u16(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, u32 value) noexcept {
	return rbc_to_chars_u32(first, last, value);
}

rbc_to_chars_result rbc_to_chars(char* first, char* last, u64 value) noexcept {
	return rbc_to_chars_u64(first, last, value);
}

#else

// NOLINTBEGIN(readability-identifier-naming)

/// Analyzes the character sequence [@p first, @p last) for a pattern described below.
/// If no characters match the pattern or if the value obtained by parsing the matched characters
/// is not representable in the type of value, @p value is unmodified;
/// otherwise the characters matching the pattern are interpreted as a text representation of an arithmetic value,
/// which is stored in @p value.
/// @param first, last 	valid character range to parse
/// @param value    	the out-parameter where the parsed value is stored if successful
	#define rbc_from_chars(first, last, value) \
		_Generic(*(value),                     \
		    i8: rbc_from_chars_i8,             \
		    i16: rbc_from_chars_i16,           \
		    i32: rbc_from_chars_i32,           \
		    i64: rbc_from_chars_i64,           \
		    u8: rbc_from_chars_u8,             \
		    u16: rbc_from_chars_u16,           \
		    u32: rbc_from_chars_u32,           \
		    u64: rbc_from_chars_u64)(first, last, value)

	#define rbc_to_chars(first, last, value) \
		_Generic((value),                    \
		    i8: rbc_to_chars_i8,             \
		    i16: rbc_to_chars_i16,           \
		    i32: rbc_to_chars_i32,           \
		    i64: rbc_to_chars_i64,           \
		    u8: rbc_to_chars_u8,             \
		    u16: rbc_to_chars_u16,           \
		    u32: rbc_to_chars_u32,           \
		    u64: rbc_to_chars_u64)(first, last, value)

// NOLINTEND(readability-identifier-naming)

#endif
