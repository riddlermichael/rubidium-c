#include "error.h"

#include <string.h>

#include <rbc/core/os.h>
#ifdef RBC_OS_WIN
	#include <Windows.h>
#endif

#include <rbc/core/macros.h>
#include <rbc/utils/charconv.h>

#define RBC_UNDEFINED_ERROR_DESC "Undefined error"

#ifdef RBC_OS_WIN

// There is no direct relationship between `GetLastError()` and `errno`,
// so some of these choices are arbitrary (and adapted from cygwin header); see
// https://cygwin.com/git/gitweb.cgi?p=newlib-cygwin.git;a=blob;f=winsup/cygwin/errno.cc
static rbc_error rbc_error_from_get_last_error(DWORD code) {
	switch (code) {
		case ERROR_SUCCESS                   : return RBC_ERROR_OK;

		case ERROR_FILE_NOT_FOUND            :
		case ERROR_PATH_NOT_FOUND            :
		case ERROR_BAD_NETPATH               :
		case ERROR_DEV_NOT_EXIST             :
		case ERROR_NETNAME_DELETED           :
		case ERROR_BAD_NET_NAME              :
		case ERROR_INVALID_NAME              :
		case ERROR_MOD_NOT_FOUND             :
		case ERROR_BAD_PATHNAME              : return RBC_ERROR_NO_SUCH_FILE_OR_DIRECTORY;

		case ERROR_TOO_MANY_OPEN_FILES       :
		case ERROR_NO_MORE_SEARCH_HANDLES    : return RBC_ERROR_TOO_MANY_FILES_OPEN_IN_SYSTEM;

		case ERROR_ACCESS_DENIED             :
		case ERROR_NETWORK_ACCESS_DENIED     : return RBC_ERROR_PERMISSION_DENIED;

		case ERROR_INVALID_HANDLE            : return RBC_ERROR_BAD_FILE_DESCRIPTOR;

		case ERROR_NOT_ENOUGH_MEMORY         :
		case ERROR_OUTOFMEMORY               : return RBC_ERROR_NOT_ENOUGH_MEMORY;

		case ERROR_INVALID_DATA              :
		case ERROR_SEEK                      :
		case ERROR_SECTOR_NOT_FOUND          :
		case ERROR_INVALID_PARAMETER         :
		case ERROR_NEGATIVE_SEEK             :
		case ERROR_BAD_ARGUMENTS             :
		case ERROR_INVALID_FLAG_NUMBER       :
		case ERROR_META_EXPANSION_TOO_LONG   :
		case ERROR_INVALID_SIGNAL_NUMBER     :
		case ERROR_THREAD_1_INACTIVE         :
		case ERROR_INVALID_EA_NAME           :
		case ERROR_EA_LIST_INCONSISTENT      : return RBC_ERROR_INVALID_ARGUMENT;

		case ERROR_INVALID_DRIVE             :
		case ERROR_BAD_UNIT                  : return RBC_ERROR_NO_SUCH_DEVICE;

		case ERROR_WRITE_PROTECT             : return RBC_ERROR_READ_ONLY_FILE_SYSTEM;

		case ERROR_CRC                       :
		case ERROR_WRITE_FAULT               :
		case ERROR_READ_FAULT                :
		case ERROR_UNEXP_NET_ERR             :
		case ERROR_NET_WRITE_FAULT           :
		case ERROR_OPEN_FAILED               :
		case ERROR_LOCK_FAILED               : return RBC_ERROR_IO_ERROR;

		case ERROR_SHARING_VIOLATION         :
		case ERROR_LOCK_VIOLATION            :
		case ERROR_NETWORK_BUSY              :
		case ERROR_BUSY_DRIVE                :
		case ERROR_PATH_BUSY                 :
		case ERROR_SIGNAL_PENDING            :
		case ERROR_BUSY                      :
		case ERROR_PIPE_BUSY                 : return RBC_ERROR_DEVICE_OR_RESOURCE_BUSY;

		case ERROR_SHARING_BUFFER_EXCEEDED   : return RBC_ERROR_NO_LOCK_AVAILABLE;
		case ERROR_HANDLE_EOF                : return RBC_ERROR_NO_MESSAGE_AVAILABLE;

		case ERROR_HANDLE_DISK_FULL          :
		case ERROR_DISK_FULL                 : return RBC_ERROR_NO_SPACE_ON_DEVICE;

		case ERROR_NOT_SUPPORTED             :
		case ERROR_BAD_NET_RESP              :
		case ERROR_CALL_NOT_IMPLEMENTED      : return RBC_ERROR_FUNCTION_NOT_SUPPORTED;

		case ERROR_FILE_EXISTS               :
		case ERROR_ALREADY_EXISTS            : return RBC_ERROR_FILE_EXISTS;

		case ERROR_CANNOT_MAKE               : return RBC_ERROR_OPERATION_NOT_PERMITTED;
		case ERROR_NO_PROC_SLOTS             : return RBC_ERROR_RESOURCE_UNAVAILABLE_TRY_AGAIN;

		case ERROR_BROKEN_PIPE               :
		case ERROR_BAD_PIPE                  :
		case ERROR_NO_DATA                   :
		case ERROR_PIPE_NOT_CONNECTED        : return RBC_ERROR_BROKEN_PIPE;

		case ERROR_BUFFER_OVERFLOW           : // hm okay
		case ERROR_LABEL_TOO_LONG            :
		case ERROR_FILENAME_EXCED_RANGE      : return RBC_ERROR_FILENAME_TOO_LONG;

		case ERROR_INSUFFICIENT_BUFFER       : return RBC_ERROR_NO_BUFFER_SPACE;
		case ERROR_WAIT_NO_CHILDREN          : return RBC_ERROR_NO_CHILD_PROCESS;
		case ERROR_DIR_NOT_EMPTY             : return RBC_ERROR_DIRECTORY_NOT_EMPTY;
		case ERROR_ATOMIC_LOCKS_NOT_SUPPORTED: return RBC_ERROR_NOT_SUPPORTED;

		case ERROR_INVALID_EXE_SIGNATURE     :
		case ERROR_EXE_MARKED_INVALID        :
		case ERROR_BAD_EXE_FORMAT            :
		case ERROR_ITERATED_DATA_EXCEEDS_64k :
		case ERROR_INVALID_MINALLOCSIZE      :
		case ERROR_IOPL_NOT_ENABLED          :
		case ERROR_INVALID_SEGDPL            :
		case ERROR_AUTODATASEG_EXCEEDS_64k   :
		case ERROR_RELOC_CHAIN_XEEDS_SEGLIM  :
		case ERROR_INFLOOP_IN_RELOC_CHAIN    :
		case ERROR_EXE_MACHINE_TYPE_MISMATCH : return RBC_ERROR_EXECUTABLE_FORMAT_ERROR;

		case ERROR_FILE_TOO_LARGE            : return RBC_ERROR_FILE_TOO_LARGE;
		case WAIT_TIMEOUT                    : return RBC_ERROR_TIMED_OUT;

	#ifndef RBC_COMPILER_GCC_LIKE
		case ERROR_DEVICE_SUPPORT_IN_PROGRESS: return RBC_ERROR_OPERATION_IN_PROGRESS;
	#endif

			// too many posts were made to a semaphore;
			// in accordance with POSIX return EOVERFLOW
		case ERROR_TOO_MANY_POSTS: return RBC_ERROR_VALUE_TOO_LARGE;

		default                  : return RBC_ERROR_UNKNOWN;
	}
}

#endif

rbc_error_code rbc_get_last_error(void) {
#ifdef RBC_OS_WIN
	return GetLastError();
#else
	return errno;
#endif
}

rbc_error rbc_error_from_error_code(rbc_error_code code) {
#ifdef RBC_OS_WIN
	return rbc_error_from_get_last_error(code);
#else
	return code >= RBC_ERROR_MIN && code <= RBC_ERROR_MAX
	         ? code
	         : RBC_ERROR_UNKNOWN;
#endif
}

rbc_error rbc_error_from_last_error(void) {
	return rbc_error_from_error_code(rbc_get_last_error());
}

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
		rbc_to_chars_result const result = rbc_to_chars(tmp, tmp + RBC_ARRAY_SIZE(tmp), (int) error);
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
