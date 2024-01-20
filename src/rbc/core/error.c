#include "error.h"

#include <string.h>

#include <rbc/core/macros.h>
#include <rbc/utils/charconv.h>

#define RBC_UNDEFINED_ERROR_DESC "Undefined error"

char const* rbc_error_to_string(rbc_error error) {
	if (error < RBC_ERROR_MIN || error > RBC_ERROR_MAX) {
		return NULL; // or "Undefined error"
	}

	if (error == RBC_ERROR_UNKNOWN) {
		return "Unknown error";
	}

	if (error == RBC_ERROR_NOT_IMPLEMENTED) {
		return "Not implemented";
	}

	if (error == RBC_ERROR_OK) {
		return "Ok";
	}

#ifdef _GNU_SOURCE
	return strerrordesc_np(error);
#else
	return NULL;
#endif
}

// NOLINTBEGIN(*-security.insecureAPI.*)

bool rbc_error_to_string_s(char* buf, usize size, rbc_error error) { // NOLINT(*-easily-swappable-parameters)
	if (!buf || !size) {
		return false;
	}

	char const* desc = rbc_error_to_string(error);
	if (!desc) {
		char tmp[RBC_TO_CHARS_RESULT_MAX_LEN + 1];
		rbc_to_chars_result const result = rbc_to_chars(tmp, tmp + RBC_ARRAY_SIZE(tmp), error);
		usize const count = result.ptr - tmp;
		usize const required_size = strlen(RBC_UNDEFINED_ERROR_DESC) + strlen(": ") + count;
		if (required_size >= size) {
			return false;
		}

		strcpy(buf, RBC_UNDEFINED_ERROR_DESC);
		strcat(buf, ": ");
		strcat(buf, tmp);
		return true;
	}

	usize const len = strlen(desc);
	if (len < size) {
		strncpy(buf, desc, len);
		return true;
	}

	if (size > 3) {
		strncpy(buf, desc, size - 4);
		strcat(buf, "...");
		return true;
	}
	return false;
}

// NOLINTEND(*-security.insecureAPI.*)
