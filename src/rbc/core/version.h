#pragma once

#include <rbc/core/attributes.h>
#include <rbc/core/export.h>
#include <rbc/core/extern.h>
#include <rbc/core/types.h>

#include <rbc/rbc_version.h>

/// Version is VVRRPPPP
#define RBC_VERSION(major, minor, patch) (((major) << 24) | ((minor) << 16) | (patch))

///
#define RBC_CURRENT_VERSION RBC_VERSION(RBC_VERSION_MAJOR, RBC_VERSION_MINOR, RBC_VERSION_PATCH)

#define RBC_COMPILER_VERSION \
	RBC_VERSION(RBC_COMPILER_VERSION_MAJOR, RBC_COMPILER_VERSION_MINOR, RBC_COMPILER_VERSION_PATCH)

#ifndef RBC_COMPILER_VERSION_STRING
	#include <rbc/core/helpers.h>

	#define RBC_MAKE_COMPILER_VERSION(major, minor, patch) \
		RBC_STRINGIFY_X(major)                             \
		"." RBC_STRINGIFY_X(minor) "." RBC_STRINGIFY_X(patch)
	#define RBC_COMPILER_VERSION_STRING \
		RBC_MAKE_COMPILER_VERSION(RBC_COMPILER_VERSION_MAJOR, RBC_COMPILER_VERSION_MINOR, RBC_COMPILER_VERSION_PATCH)
#endif

///
struct RbcVersion {
	unsigned major;
	unsigned minor;
	unsigned patch;
};
typedef struct RbcVersion RbcVersion;

///
enum RbcVersionComponent {
	RBC_VERSION_COMPONENT_MAJOR,
	RBC_VERSION_COMPONENT_MINOR,
	RBC_VERSION_COMPONENT_PATCH,
};
typedef enum RbcVersionComponent RbcVersionComponent;

RBC_BEGIN_EXTERN_C

RBC_EXPORT RBC_CONST RBC_NOTHROW RbcVersion rbc_version_unpack(u32 value);
RBC_EXPORT RBC_CONST RBC_NOTHROW u32 rbc_version_pack(RbcVersion version);
RBC_EXPORT RBC_CONST RBC_NOTHROW RbcVersion rbc_version_bump(RbcVersion version, RbcVersionComponent component);

RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_eq(RbcVersion lhs, RbcVersion rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_ne(RbcVersion lhs, RbcVersion rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_lt(RbcVersion lhs, RbcVersion rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_le(RbcVersion lhs, RbcVersion rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_gt(RbcVersion lhs, RbcVersion rhs);
RBC_EXPORT RBC_CONST RBC_NOTHROW bool rbc_version_ge(RbcVersion lhs, RbcVersion rhs);

RBC_END_EXTERN_C
