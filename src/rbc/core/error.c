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
	// EOPNOTSUPP and ENOTSUP may be identical
	if (error == RBC_ERROR_NOT_SUPPORTED) {
		return "Operation not supported";
	}

	// EAGAIN and EWOULDBLOCK may be identical; we choose message for EAGAIN
	if (error == RBC_ERROR_RESOURCE_UNAVAILABLE_TRY_AGAIN) {
		return "Resource unavailable, try again";
	}

	switch (error) {
		case RBC_ERROR_UNKNOWN                           : return "Unknown error";
		case RBC_ERROR_NOT_IMPLEMENTED                   : return "Not implemented";
		case RBC_ERROR_OK                                : return "Ok";
		case RBC_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED      : return "Address family not supported";
		case RBC_ERROR_ADDRESS_IN_USE                    : return "Address in use";
		case RBC_ERROR_ADDRESS_NOT_AVAILABLE             : return "Address not available";
		case RBC_ERROR_ALREADY_CONNECTED                 : return "Socket is connected";
		case RBC_ERROR_ARGUMENT_LIST_TOO_LONG            : return "Argument list too long";
		case RBC_ERROR_ARGUMENT_OUT_OF_DOMAIN            : return "Mathematics argument out of domain of function";
		case RBC_ERROR_BAD_ADDRESS                       : return "Bad address";
		case RBC_ERROR_BAD_FILE_DESCRIPTOR               : return "Bad file descriptor";
		case RBC_ERROR_BAD_MESSAGE                       : return "Bad message";
		case RBC_ERROR_BROKEN_PIPE                       : return "Broken pipe";
		case RBC_ERROR_CONNECTION_ABORTED                : return "Connection aborted";
		case RBC_ERROR_CONNECTION_ALREADY_IN_PROGRESS    : return "Connection already in progress";
		case RBC_ERROR_CONNECTION_REFUSED                : return "Connection refused";
		case RBC_ERROR_CONNECTION_RESET                  : return "Connection reset";
		case RBC_ERROR_CROSS_DEVICE_LINK                 : return "Improper link";
		case RBC_ERROR_DESTINATION_ADDRESS_REQUIRED      : return "Destination address required";
		case RBC_ERROR_DEVICE_OR_RESOURCE_BUSY           : return "Device or resource busy";
		case RBC_ERROR_DIRECTORY_NOT_EMPTY               : return "Directory not empty";
		case RBC_ERROR_EXECUTABLE_FORMAT_ERROR           : return "Executable file format error";
		case RBC_ERROR_FILE_EXISTS                       : return "File exists";
		case RBC_ERROR_FILE_TOO_LARGE                    : return "File too large";
		case RBC_ERROR_FILENAME_TOO_LONG                 : return "Filename too long";
		case RBC_ERROR_FUNCTION_NOT_SUPPORTED            : return "Function not supported";
		case RBC_ERROR_HOST_UNREACHABLE                  : return "Host is unreachable";
		case RBC_ERROR_IDENTIFIER_REMOVED                : return "Identifier removed";
		case RBC_ERROR_ILLEGAL_BYTE_SEQUENCE             : return "Illegal byte sequence";
		case RBC_ERROR_INAPPROPRIATE_IO_CONTROL_OPERATION: return "Inappropriate I/O control operation";
		case RBC_ERROR_INTERRUPTED                       : return "Interrupted function call";
		case RBC_ERROR_INVALID_ARGUMENT                  : return "Invalid argument";
		case RBC_ERROR_INVALID_SEEK                      : return "Invalid seek";
		case RBC_ERROR_IO_ERROR                          : return "Input/output error";
		case RBC_ERROR_IS_A_DIRECTORY                    : return "Is a directory";
		case RBC_ERROR_MESSAGE_SIZE                      : return "Message too long";
		case RBC_ERROR_NETWORK_DOWN                      : return "Network is down";
		case RBC_ERROR_NETWORK_RESET                     : return "Connection aborted by network";
		case RBC_ERROR_NETWORK_UNREACHABLE               : return "Network unreachable";
		case RBC_ERROR_NO_BUFFER_SPACE                   : return "No buffer space available";
		case RBC_ERROR_NO_CHILD_PROCESS                  : return "No child processes";
		case RBC_ERROR_NO_LINK                           : return "Link has been severed";
		case RBC_ERROR_NO_LOCK_AVAILABLE                 : return "No locks available";
		case RBC_ERROR_NO_MESSAGE                        : return "No message of the desired type";
		case RBC_ERROR_NO_PROTOCOL_OPTION                : return "Protocol not available";
		case RBC_ERROR_NO_SPACE_ON_DEVICE                : return "No space left on device";
		case RBC_ERROR_NO_STREAM_RESOURCES               : return "No stream resources";
		case RBC_ERROR_NO_SUCH_DEVICE_OR_ADDRESS         : return "No such device or address";
		case RBC_ERROR_NO_SUCH_DEVICE                    : return "No such device";
		case RBC_ERROR_NO_SUCH_FILE_OR_DIRECTORY         : return "No such file or directory";
		case RBC_ERROR_NO_SUCH_PROCESS                   : return "No such process";
		case RBC_ERROR_NOT_A_DIRECTORY                   : return "Not a directory";
		case RBC_ERROR_NOT_A_SOCKET                      : return "Not a socket";
		case RBC_ERROR_NOT_A_STREAM                      : return "Not a stream";
		case RBC_ERROR_NOT_CONNECTED                     : return "The socket is not connected";
		case RBC_ERROR_NOT_ENOUGH_MEMORY                 : return "Not enough space";
		case RBC_ERROR_OPERATION_CANCELED                : return "Operation canceled";
		case RBC_ERROR_OPERATION_IN_PROGRESS             : return "Operation in progress";
		case RBC_ERROR_OPERATION_NOT_PERMITTED           : return "Operation not permitted";
		case RBC_ERROR_OPERATION_NOT_SUPPORTED           : return "Operation not supported on socket";
		case RBC_ERROR_OPERATION_WOULD_BLOCK             : return "Operation would block";
		case RBC_ERROR_OWNER_DEAD                        : return "Owner died";
		case RBC_ERROR_PERMISSION_DENIED                 : return "Permission denied";
		case RBC_ERROR_PROTOCOL_ERROR                    : return "Protocol error";
		case RBC_ERROR_PROTOCOL_NOT_SUPPORTED            : return "Protocol not supported";
		case RBC_ERROR_READ_ONLY_FILE_SYSTEM             : return "Read-only filesystem";
		case RBC_ERROR_RESOURCE_DEADLOCK_WOULD_OCCUR     : return "Resource deadlock would occur";
		case RBC_ERROR_RESULT_OUT_OF_RANGE               : return "Result too large";
		case RBC_ERROR_STATE_NOT_RECOVERABLE             : return "State not recoverable";
		// STREAM ioctl(2) timeout (POSIX.1, C++)
		case RBC_ERROR_STREAM_TIMEOUT                    : return "Timer expired";
		case RBC_ERROR_TEXT_FILE_BUSY                    : return "Text file busy";
		case RBC_ERROR_TIMED_OUT                         : return "Connection timed out";
		// on Linux, this is probably a result of encountering the /proc/sys/fs/file-max limit (see proc(5))
		case RBC_ERROR_TOO_MANY_FILES_OPEN_IN_SYSTEM     : return "Too many open files in system";
		/* Commonly caused by exceeding the RLIMIT_NOFILE resource limit described in getrlimit(2).
 * Can also be caused by exceeding the limit specified in /proc/sys/fs/nr_open. */
		// File descriptor value too large (in C++)
		case RBC_ERROR_TOO_MANY_FILES_OPEN               : return "Too many open files";
		case RBC_ERROR_TOO_MANY_LINKS                    : return "Too many links";
		case RBC_ERROR_TOO_MANY_SYMBOLIC_LINK_LEVELS     : return "Too many levels of symbolic links";
		case RBC_ERROR_VALUE_TOO_LARGE                   : return "Value too large to be stored in data type";
		case RBC_ERROR_WRONG_PROTOCOL_TYPE               : return "Protocol wrong type for socket";

		// very long description breaks formatting
		case RBC_ERROR_NO_MESSAGE_AVAILABLE:
			return "The named attribute does not exist, or the process has no access to this attribute";

		default: return "Undefined error";
	}
}

// NOLINTBEGIN(*-security.insecureAPI.*)

bool rbc_error_to_string_s(char* buf, usize size, rbc_error error) { // NOLINT(*-easily-swappable-parameters)
	if (!buf || !size) {
		return false;
	}

	char const* desc = rbc_error_to_string(error);
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
