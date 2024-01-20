#pragma once

#include <rbc/core/compiler.h>

#ifdef RBC_COMPILER_MSVC
	#include <sal.h>
#endif

#ifdef __has_attribute
	#define RBC_HAS_ATTRIBUTE(...) __has_attribute(__VA_ARGS__)
#else
	#define RBC_HAS_ATTRIBUTE(...) 0
#endif

#ifdef __has_c_attribute
	#define RBC_HAS_C_ATTRIBUTE(...) __has_c_attribute(__VA_ARGS__)
#else
	#define RBC_HAS_C_ATTRIBUTE(...) 0
#endif

#ifdef __has_declspec_attribute
	#define RBC_HAS_DECLSPEC_ATTRIBUTE(...) __has_declspec_attribute(__VA_ARGS__)
#else
	#define RBC_HAS_DECLSPEC_ATTRIBUTE(...) 0
#endif

#if defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_DEPRECATED __attribute__((deprecated))
	#define RBC_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
	#define RBC_NODISCARD __attribute__((warn_unused_result))
	#define RBC_MAYBE_UNUSED __attribute__((unused))
#elif defined(RBC_COMPILER_MSVC)
	#define RBC_DEPRECATED __declspec(deprecated)
	#define RBC_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
	#define RBC_NODISCARD _Check_return_
// VS 2017 15.3, /std:c++17
	#if _MSC_VER >= 1911 && _MSVC_LANG >= 201703L
		#define RBC_MAYBE_UNUSED [[maybe_unused]]
	#else
		#define RBC_MAYBE_UNUSED
	#endif
#else
	#define RBC_DEPRECATED
	#define RBC_DEPRECATED_MSG(msg)
	#define RBC_NODISCARD
	#define RBC_MAYBE_UNUSED
#endif

#if defined(RBC_COMPILER_GCC_LIKE)
// Clang also allows the nonnull attribute to be placed directly on a function parameter;
// use `RBC_ARG_NONNULL` in a such case
	#define RBC_NONNULL __attribute__((nonnull))
	#define RBC_NONNULL_ARGS(...) __attribute__((nonnull(__VA_ARGS__)))
	#define RBC_RETURNS_NONNULL __attribute__((returns_nonnull))
	#define RBC_ALLOCATOR __attribute__((malloc))
// `alloc_size(N)` implies that argument number `N` equals the number of available bytes at the returned pointer
// (used for `malloc`-like functions).
// `alloc_size(N, M)` implies that the product of argument number `N` and argument number `M`
// equals the number of available bytes at the returned pointer (used for `calloc`-like functions).
// Argument numbers are 1-based.
	#define RBC_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#elif defined(RBC_COMPILER_MSVC)
	#define RBC_NONNULL
	#define RBC_NONNULL_ARGS(...)
	#define RBC_RETURNS_NONNULL _Ret_notnull_
	#define RBC_ALLOCATOR __declspec(allocator)
	#define RBC_ALLOC_SIZE(...)
#else
	#define RBC_NONNULL
	#define RBC_NONNULL_ARGS(...)
	#define RBC_RETURNS_NONNULL
	#define RBC_ALLOCATOR
	#define RBC_ALLOC_SIZE(...)
#endif

/**
@def RBC_NOALIAS
The `RBC_NOALIAS` attribute indicates that the only memory accesses inside function
are loads and stores from objects pointed to by its pointer-typed arguments, with arbitrary offsets.
`RBC_NOALIAS` means that a function call doesn't modify or reference visible global state
and only modifies the memory pointed to _directly_ by pointer parameters (first-level indirections).
*/
#if defined(RBC_COMPILER_MSVC)
	#define RBC_NOALIAS __declspec(noalias)
#elif defined(RBC_COMPILER_CLANG)
	#if RBC_HAS_DECLSPEC_ATTRIBUTE(noalias)
		#define RBC_NOALIAS __declspec(noalias)
	#else
		#define RBC_NOALIAS
	#endif
#else
	#define RBC_NOALIAS
#endif

// externally_visible:
// This attribute, attached to a global variable or function nullify effect of `-fwhole-program` command line option,
// so the object remains visible outside the current compilation unit
//
// -fwhole-program:
// Assume that the current compilation unit represents the whole program being compiled.
// All public functions and variables except main and those merged by attribute `externally_visible` become static functions
// and in effect are optimized more aggressively by inter-procedural optimizers.
#if defined(RBC_COMPILER_GCC) || defined(RBC_COMPILER_MINGW)
	#define RBC_EXTERNALLY_VISIBLE __attribute__((externally_visible))
#else
	#define RBC_EXTERNALLY_VISIBLE
#endif

#if defined(RBC_COMPILER_MSVC)
	#define RBC_ARG_NONNULL _Notnull_
#elif defined(RBC_COMPILER_CLANG)
	#define RBC_ARG_NONNULL __attribute__((nonnull))
#else
	#define RBC_ARG_NONNULL
#endif

#if defined(RBC_COMPILER_GCC_LIKE)
    /**
     * Calls to functions whose return value is not affected by changes to the observable state of the program
     * and that have no observable effects on such state other than to return a value may lend themselves to optimizations
     * such as common subexpression elimination.
     * Declaring such functions with the `const` attribute allows to avoid emitting some calls
     * in repeated invocations of the function with the same argument values.
     *
     * The `const` attribute prohibits a function from reading objects that affect its return value
     * between successive invocations.
     * However, functions declared with the attribute can safely read objects that do not change their return value,
     * such as non-volatile constants.
     *
     * The `const` attribute imposes greater restrictions on a function’s definition than the similar `pure` attribute.
     * Because a `const` function cannot have any observable side effects it does not make sense for it to return `void`.
     *
     * Note that a function that has pointer arguments and examines the data pointed to **must not be** declared `const`
     * if the pointed-to data might change between successive invocations of the function.
     * In general, since a function cannot distinguish data that might change from data that cannot,
     * `const` functions should never take pointer or, in C++, reference arguments.
     * Likewise, a function that calls a non-const function usually must not be `const` itself.
     */
	#define RBC_CONST __attribute__((const))

    /**
     * Calls to functions that have no observable effects on the state of the program other than to return a value
     * may lend themselves to optimizations such as common subexpression elimination.
     * Declaring such functions with the `pure` attribute allows to avoid emitting some calls
     * in repeated invocations of the function with the same argument values.
     *
     * The `pure` attribute prohibits a function from modifying the state of the program that is observable
     * by means other than inspecting the function’s return value.
     * However, functions declared with the `pure` attribute can safely read any non-volatile objects,
     * and modify the value of objects in a way that does not affect their return value
     * or the observable state of the program.
     *
     * Some common examples of pure functions are `strlen` or `memcmp`.
     * Interesting non-`pure` functions are functions with infinite loops or those depending on volatile memory
     * or other system resource, that may change between consecutive calls
     * (such as the standard C `feof` function in a multithreading environment).
     *
     * The pure attribute imposes similar but looser restrictions on a function’s definition than the `const` attribute:
     * `pure` allows the function to read any non-volatile memory, even if it changes in between successive invocations
     * of the function.
     * Because a `pure` function cannot have any observable side effects it does not make sense for such a function
     * to return `void`.
     */
	#define RBC_PURE __attribute__((pure))
#else
	#define RBC_CONST
	#define RBC_PURE
#endif

#if defined(RBC_COMPILER_GCC_LIKE)
	#define RBC_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(RBC_COMPILER_MSVC)
	#define RBC_ALWAYS_INLINE __forceinline
#else
	#define RBC_ALWAYS_INLINE inline
#endif

#if RBC_HAS_ATTRIBUTE(cleanup)
	#define RBC_CLEANUP(dtor) __attribute__((cleanup(dtor)))
#else
	#define RBC_CLEANUP(dtor)
#endif

// SAL annotations

#ifdef RBC_COMPILER_MSVC
	#define RBC_IN _In_
#else
	#define RBC_IN
#endif
