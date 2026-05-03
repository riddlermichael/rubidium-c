#pragma once

/**
 * The operating system, must be one of (RBC_OS_*):
 * AIX      - AIX
 * ANDROID  - Android platform (also RBC_OS_LINUX defined)
 * CYGWIN   - Cygwin
 * DARWIN   - Any Darwin system (macOS, iOS, watchOS, tvOS)
 * FREEBSD  - FreeBSD
 * HAIKU    - Haiku
 * HPUX     - HP-UX
 * HURD     - GNU Hurd
 * INTERIX  - Interix
 * IOS      - iOS
 * LINUX    - Linux
 * LYNX     - LynxOS
 * MACOS    - macOS
 * NETBSD   - NetBSD
 * OPENBSD  - OpenBSD
 * QNX      - QNX
 * QNX6     - QNX RTP 6.1
 * SOLARIS  - Sun Solaris
 * TVOS     - tvOS
 * WATCHOS  - watchOS
 * WIN32    - Windows (WIN64 on 64-bit platforms also defined)
 * WINRT    - WinRT
 */

#if defined(__APPLE__)
	#include <TargetConditionals.h>

	#define RBC_OS_DARWIN

	#if defined(TARGET_OS_OSX) && TARGET_OS_OSX
		#define RBC_OS_MACOS
	#elif defined(TARGET_OS_IOS) && TARGET_OS_IOS
		#define RBC_OS_IOS
	#elif defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
		#define RBC_OS_WATCHOS
	#elif defined(TARGET_OS_TV) && TARGET_OS_TV
		#define RBC_OS_TVOS
	#elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
		#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
			#define RBC_OS_IOS
		#else
			#define RBC_OS_MACOS
		#endif
	#else
		#error "Unknown Apple platform"
	#endif

	#ifdef __LP64__
		#define RBC_OS_DARWIN64
	#else
		#define RBC_OS_DARWIN32
	#endif
#elif defined(__ANDROID__) || defined(ANDROID)
	#define RBC_OS_ANDROID
	#define RBC_OS_LINUX
#elif defined(__CYGWIN__)
	#define RBC_OS_CYGWIN
#elif defined(_WIN32) || defined(_WIN64) || defined(__NT__)
	#define RBC_OS_WIN32

	#if defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
		#define RBC_OS_WIN64
	#endif

	#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)
		#define RBC_OS_WINRT
	#endif
#elif defined(__sun) || defined(sun)
	#define RBC_OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
	#define RBC_OS_HPUX
#elif defined(__native_client__)
	#define RBC_OS_NACL
#elif defined(__EMSCRIPTEN__)
	#define RBC_OS_WASM
#elif defined(__linux__) || defined(__linux)
	#define RBC_OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
	#define RBC_OS_FREEBSD
#elif defined(__NetBSD__)
	#define RBC_OS_NETBSD
#elif defined(__OpenBSD__)
	#define RBC_OS_OPENBSD
#elif defined(__INTERIX)
	#define RBC_OS_INTERIX
#elif defined(_AIX)
	#define RBC_OS_AIX
#elif defined(__Lynx__)
	#define RBC_OS_LYNX
#elif defined(__GNU__)
	#define RBC_OS_HURD
#elif defined(__QNXNTO__)
	#define RBC_OS_QNX
#elif defined(__INTEGRITY)
	#define RBC_OS_INTEGRITY
#elif defined(__HAIKU__)
	#define RBC_OS_HAIKU
#else
	#error "Unsupported OS"
#endif

#if defined(RBC_OS_WIN32) || defined(RBC_OS_WIN64) || defined(RBC_OS_WINRT) \
    || defined(RBC_OS_CYGWIN)
	#define RBC_OS_WIN
#endif
