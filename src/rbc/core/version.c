#include "version.h"

#include <rbc/core/builtins.h>

RbcVersion rbc_version_unpack(u32 value) {
	u32 const major = (value & 0xFF000000) >> 24;
	u32 const minor = (value & 0x00FF0000) >> 16;
	u32 const patch = value & 0xFFFF;
	return (RbcVersion) {.major = major, .minor = minor, .patch = patch};
}

u32 rbc_version_pack(RbcVersion version) {
	return ((version.major & 0xFF) << 24)
	    | ((version.minor & 0xFF) << 16)
	    | (version.patch & 0xFFFF);
}

RbcVersion rbc_version_bump(RbcVersion version, RbcVersionComponent component) {
	switch (component) {
		case RBC_VERSION_COMPONENT_MAJOR: return (RbcVersion) {.major = version.major + 1, .minor = 0, .patch = 0};
		case RBC_VERSION_COMPONENT_MINOR: return (RbcVersion) {.major = version.major, .minor = version.minor + 1, .patch = 0};
		case RBC_VERSION_COMPONENT_PATCH: return (RbcVersion) {.major = version.major, .minor = version.minor, .patch = version.patch + 1};
		default                         : RBC_UNREACHABLE();
	}
}

bool rbc_version_eq(RbcVersion lhs, RbcVersion rhs) {
	return lhs.major == rhs.major
	    && lhs.minor == rhs.minor
	    && lhs.patch == rhs.patch;
}

bool rbc_version_ne(RbcVersion lhs, RbcVersion rhs) {
	return !rbc_version_eq(lhs, rhs);
}

bool rbc_version_lt(RbcVersion lhs, RbcVersion rhs) {
	if (lhs.major < rhs.major) {
		return true;
	}

	if (lhs.major > rhs.major) {
		return false;
	}

	if (lhs.minor < rhs.minor) {
		return true;
	}

	if (lhs.minor > rhs.minor) {
		return false;
	}

	return lhs.patch < rhs.patch;
}

bool rbc_version_le(RbcVersion lhs, RbcVersion rhs) {
	return !rbc_version_lt(rhs, lhs); // NOLINT(*-suspicious-call-argument)
}

bool rbc_version_gt(RbcVersion lhs, RbcVersion rhs) {
	return rbc_version_lt(rhs, lhs); // NOLINT(*-suspicious-call-argument)
}

bool rbc_version_ge(RbcVersion lhs, RbcVersion rhs) {
	return !rbc_version_lt(lhs, rhs);
}
