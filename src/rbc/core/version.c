#include "version.h"

#include <rbc/core/builtins.h>

rbc_version rbc_version_unpack(u32 value) {
	u32 const major = (value & 0xFF000000) >> 24;
	u32 const minor = (value & 0x00FF0000) >> 16;
	u32 const patch = value & 0xFFFF;
	return (rbc_version){major, minor, patch};
}

u32 rbc_version_pack(rbc_version version) {
	return ((version.major & 0xFF) << 24) | ((version.minor & 0xFF) << 16) | (version.patch & 0xFFFF);
}

rbc_version rbc_version_bump(rbc_version version, rbc_version_component component) {
	switch (component) {
		case RBC_VERSION_COMPONENT_MAJOR: return (rbc_version){version.major + 1, 0, 0};
		case RBC_VERSION_COMPONENT_MINOR: return (rbc_version){version.major, version.minor + 1, 0};
		case RBC_VERSION_COMPONENT_PATCH: return (rbc_version){version.major, version.minor, version.patch + 1};
		default                         : RBC_UNREACHABLE();
	}
}

bool rbc_version_eq(rbc_version lhs, rbc_version rhs) {
	return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch;
}

bool rbc_version_ne(rbc_version lhs, rbc_version rhs) {
	return !rbc_version_eq(lhs, rhs);
}

bool rbc_version_lt(rbc_version lhs, rbc_version rhs) {
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

bool rbc_version_le(rbc_version lhs, rbc_version rhs) {
	return !rbc_version_lt(rhs, lhs); // NOLINT(*-suspicious-call-argument)
}

bool rbc_version_gt(rbc_version lhs, rbc_version rhs) {
	return rbc_version_lt(rhs, lhs); // NOLINT(*-suspicious-call-argument)
}

bool rbc_version_ge(rbc_version lhs, rbc_version rhs) {
	return !rbc_version_lt(lhs, rhs);
}
