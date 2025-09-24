#include "charconv.h"

#include <ctype.h>

#include <rbc/core/macros.h>

// NOLINTBEGIN(readability-identifier-naming)

#define i8min INT8_MIN
#define i16min INT16_MIN
#define i32min INT32_MIN
#define i64min INT64_MIN

#define i8max INT8_MAX
#define i16max INT16_MAX
#define i32max INT32_MAX
#define i64max INT64_MAX

#define u8max UINT8_MAX
#define u16max UINT16_MAX
#define u32max UINT32_MAX
#define u64max UINT64_MAX

// NOLINTEND(readability-identifier-naming)

// NOLINTBEGIN(bugprone-macro-parentheses)

#define RBC_FROM_CHARS_IMPL_SIGNED(type)                                      \
	rbc_from_chars_result RBC_CONCAT(rbc_from_chars_, type)(                  \
	    char const* first,                                                    \
	    char const* last,                                                     \
	    type* value) {                                                        \
		type const max = RBC_CONCAT(type, max);                               \
		type const min = RBC_CONCAT(type, min);                               \
		rbc_from_chars_result result;                                         \
		result.ptr = first;                                                   \
		result.error = RBC_ERROR_INVALID_ARGUMENT;                            \
		if (last <= first) {                                                  \
			return result;                                                    \
		}                                                                     \
                                                                              \
		while (first != last && isspace(*first)) {                            \
			++first;                                                          \
		}                                                                     \
		if (first == last) {                                                  \
			return result;                                                    \
		}                                                                     \
                                                                              \
		bool is_negative = false;                                             \
		if (*first == '+' || *first == '-') {                                 \
			is_negative = *first == '-';                                      \
			++first;                                                          \
		}                                                                     \
		if (first == last || !isdigit(*first)) {                              \
			return result;                                                    \
		}                                                                     \
                                                                              \
		result.error = RBC_ERROR_RESULT_OUT_OF_RANGE;                         \
		type x = *first - '0';                                                \
		bool is_min = false;                                                  \
		for (++first; first != last && isdigit(*first); ++first) {            \
			if (is_min || x > max / 10) {                                     \
				result.ptr = first;                                           \
				return result;                                                \
			}                                                                 \
                                                                              \
			x *= 10;                                                          \
			type d = *first - '0';                                            \
			if (!is_negative && x > max - d || is_negative && -x < min + d) { \
				result.ptr = first;                                           \
				return result;                                                \
			}                                                                 \
                                                                              \
			is_min = -x == min + d;                                           \
			x += d;                                                           \
		}                                                                     \
                                                                              \
		if (is_negative) {                                                    \
			x = is_min ? min : -x;                                            \
		}                                                                     \
		*value = x;                                                           \
		result.ptr = first;                                                   \
		result.error = RBC_ERROR_OK;                                          \
		return result;                                                        \
	}

RBC_FROM_CHARS_IMPL_SIGNED(i8)
RBC_FROM_CHARS_IMPL_SIGNED(i16)
RBC_FROM_CHARS_IMPL_SIGNED(i32)
RBC_FROM_CHARS_IMPL_SIGNED(i64)

#define RBC_FROM_CHARS_IMPL_UNSIGNED(type)                         \
	rbc_from_chars_result RBC_CONCAT(rbc_from_chars_, type)(       \
	    char const* first,                                         \
	    char const* last,                                          \
	    type* value) {                                             \
		type const max = RBC_CONCAT(type, max);                    \
		rbc_from_chars_result result;                              \
		result.ptr = first;                                        \
		result.error = RBC_ERROR_INVALID_ARGUMENT;                 \
		if (last <= first) {                                       \
			return result;                                         \
		}                                                          \
                                                                   \
		while (first != last && isspace(*first)) {                 \
			++first;                                               \
		}                                                          \
		if (first == last || *first == '-') {                      \
			return result;                                         \
		}                                                          \
                                                                   \
		if (*first == '+') {                                       \
			++first;                                               \
		}                                                          \
		if (first == last || !isdigit(*first)) {                   \
			return result;                                         \
		}                                                          \
                                                                   \
		result.error = RBC_ERROR_RESULT_OUT_OF_RANGE;              \
		type x = *first - '0';                                     \
		for (++first; first != last && isdigit(*first); ++first) { \
			if (x > max / 10) {                                    \
				result.ptr = first;                                \
				return result;                                     \
			}                                                      \
                                                                   \
			x *= 10;                                               \
			type d = *first - '0';                                 \
			if (x > max - d) {                                     \
				result.ptr = first;                                \
				return result;                                     \
			}                                                      \
                                                                   \
			x += d;                                                \
		}                                                          \
                                                                   \
		*value = x;                                                \
		result.ptr = first;                                        \
		result.error = RBC_ERROR_OK;                               \
		return result;                                             \
	}

RBC_FROM_CHARS_IMPL_UNSIGNED(u8)
RBC_FROM_CHARS_IMPL_UNSIGNED(u16)
RBC_FROM_CHARS_IMPL_UNSIGNED(u32)
RBC_FROM_CHARS_IMPL_UNSIGNED(u64)

#define RBC_TO_CHARS_IMPL_SIGNED(type)                                        \
	rbc_to_chars_result RBC_CONCAT(rbc_to_chars_, type)(                      \
	    char* first,                                                          \
	    char* last,                                                           \
	    type value) {                                                         \
		if (last <= first) {                                                  \
			return (rbc_to_chars_result) {first, RBC_ERROR_INVALID_ARGUMENT}; \
		}                                                                     \
                                                                              \
		if (value == 0) {                                                     \
			*first++ = '0';                                                   \
			return (rbc_to_chars_result) {first, RBC_ERROR_OK};               \
		}                                                                     \
                                                                              \
		rbc_to_chars_result result;                                           \
		result.ptr = last;                                                    \
		result.error = RBC_ERROR_VALUE_TOO_LARGE;                             \
                                                                              \
		bool const is_min = value == RBC_CONCAT(type, min);                   \
		if (value < 0) {                                                      \
			*first++ = '-';                                                   \
			value = is_min ? RBC_CONCAT(type, max) : -value;                  \
		}                                                                     \
		if (first == last) {                                                  \
			return result;                                                    \
		}                                                                     \
                                                                              \
		int nb_digits = 0;                                                    \
		for (type x = value; x; x /= 10) {                                    \
			++nb_digits;                                                      \
		}                                                                     \
		if (last < first + nb_digits) {                                       \
			return result;                                                    \
		}                                                                     \
                                                                              \
		char* ptr = first;                                                    \
		while (value) {                                                       \
			*ptr++ = '0' + (value % 10);                                      \
			value /= 10;                                                      \
		}                                                                     \
		if (is_min) {                                                         \
			++*first;                                                         \
		}                                                                     \
                                                                              \
		result.ptr = ptr;                                                     \
		result.error = RBC_ERROR_OK;                                          \
		for (last = first + nb_digits - 1; first < last; ++first, --last) {   \
			char t = *first;                                                  \
			*first = *last;                                                   \
			*last = t;                                                        \
		}                                                                     \
		return result;                                                        \
	}

RBC_TO_CHARS_IMPL_SIGNED(i8)
RBC_TO_CHARS_IMPL_SIGNED(i16)
RBC_TO_CHARS_IMPL_SIGNED(i32)
RBC_TO_CHARS_IMPL_SIGNED(i64)

#define RBC_TO_CHARS_IMPL_UNSIGNED(type)                                      \
	rbc_to_chars_result RBC_CONCAT(rbc_to_chars_, type)(                      \
	    char* first,                                                          \
	    char* last,                                                           \
	    type value) {                                                         \
		if (last <= first) {                                                  \
			return (rbc_to_chars_result) {first, RBC_ERROR_INVALID_ARGUMENT}; \
		}                                                                     \
                                                                              \
		if (value == 0) {                                                     \
			*first++ = '0';                                                   \
			return (rbc_to_chars_result) {first, RBC_ERROR_OK};               \
		}                                                                     \
                                                                              \
		rbc_to_chars_result result;                                           \
		result.ptr = last;                                                    \
		result.error = RBC_ERROR_VALUE_TOO_LARGE;                             \
		int nb_digits = 0;                                                    \
		for (type x = value; x; x /= 10) {                                    \
			++nb_digits;                                                      \
		}                                                                     \
		if (last < first + nb_digits) {                                       \
			return result;                                                    \
		}                                                                     \
                                                                              \
		char* ptr = first;                                                    \
		while (value) {                                                       \
			*ptr++ = '0' + (value % 10);                                      \
			value /= 10;                                                      \
		}                                                                     \
                                                                              \
		result.ptr = ptr;                                                     \
		result.error = RBC_ERROR_OK;                                          \
		for (last = first + nb_digits - 1; first < last; ++first, --last) {   \
			char t = *first;                                                  \
			*first = *last;                                                   \
			*last = t;                                                        \
		}                                                                     \
		return result;                                                        \
	}

RBC_TO_CHARS_IMPL_UNSIGNED(u8)
RBC_TO_CHARS_IMPL_UNSIGNED(u16)
RBC_TO_CHARS_IMPL_UNSIGNED(u32)
RBC_TO_CHARS_IMPL_UNSIGNED(u64)

// NOLINTEND(bugprone-macro-parentheses)
