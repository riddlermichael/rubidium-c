if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows|CYGWIN")
    set(HOST_SYSTEM_NAME windows)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    set(HOST_SYSTEM_NAME posix)
else()
    message(FATAL_ERROR "Unsupported host system")
endif()
message(VERBOSE "host system: ${HOST_SYSTEM_NAME}")

if(CMAKE_SYSTEM_NAME MATCHES "Windows|CYGWIN")
    set(SYSTEM_NAME windows)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(SYSTEM_NAME posix)
else()
    message(FATAL_ERROR "Unsupported target system")
endif()
message(VERBOSE "target system: ${SYSTEM_NAME}")
