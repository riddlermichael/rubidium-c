#pragma once

#include <rbc/core/attributes.h>
#include <rbc/core/export.h>
#include <rbc/core/extern.h>
#include <rbc/core/keywords.h>
#include <rbc/core/types.h>

#include <rbc/rbc_version.h>

/// Version is VVRRPPPP
#define RBC_VERSION(major, minor, patch) (((major) << 24) | ((minor) << 16) | (patch))

///
#define RBC_CURRENT_VERSION RBC_VERSION(RBC_VERSION_MAJOR, RBC_VERSION_MINOR, RBC_VERSION_PATCH)

#define RBC_COMPILER_VERSION \
	RBC_VERSION(RBC_COMPILER_VERSION_MAJOR, RBC_COMPILER_VERSION_MINOR, RBC_COMPILER_VERSION_PATCH)

#ifndef RBC_COMPILER_VERSION_STRING
	#define RBC_MAKE_COMPILER_VERSION(major, minor, patch) \
		RBC_STRINGIFY_X(major)                             \
		"." RBC_STRINGIFY_X(minor) "." RBC_STRINGIFY_X(patch)
	#define RBC_COMPILER_VERSION_STRING \
		RBC_MAKE_COMPILER_VERSION(RBC_COMPILER_VERSION_MAJOR, RBC_COMPILER_VERSION_MINOR, RBC_COMPILER_VERSION_PATCH)
#endif

///
struct rbc_version {
	unsigned major;
	unsigned minor;
	unsigned patch;
};
typedef struct rbc_version rbc_version;

///
enum rbc_version_component {
	RBC_VERSION_COMPONENT_MAJOR,
	RBC_VERSION_COMPONENT_MINOR,
	RBC_VERSION_COMPONENT_PATCH,
};
typedef enum rbc_version_component rbc_version_component;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RBC_CONST RBC_NOTHROW rbc_version rbc_version_unpack(u32 value);
RBC_EXPORT RBC_CONST RBC_NOTHROW u32 rbc_version_pack(rbc_version version);
RBC_EXPORT RBC_CONST RBC_NOTHROW rbc_version rbc_version_bump(rbc_version version, rbc_version_component component);

RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_eq(rbc_version lhs, rbc_version rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_ne(rbc_version lhs, rbc_version rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_lt(rbc_version lhs, rbc_version rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_le(rbc_version lhs, rbc_version rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_gt(rbc_version lhs, rbc_version rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_ge(rbc_version lhs, rbc_version rhs);

RBC_END_EXTERN_C
