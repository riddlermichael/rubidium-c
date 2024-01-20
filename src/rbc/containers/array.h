#pragma once

#include <rbc/containers/macros.h>
#include <rbc/core/attributes.h>
#include <rbc/core/export.h>
#include <rbc/core/types.h>

#define RBC_ARRAY_TYPE_NAME(type) rbc_array_##type

#define RBC_ARRAY_DEF_GENERIC(name, type, export)                \
	struct name {                                                \
		type* data;                                              \
		usize size;                                              \
	};                                                           \
                                                                 \
	typedef struct name name;                                    \
                                                                 \
	export RBC_NODISCARD name RBC_METHOD(name, new)(usize size); \
	export void RBC_METHOD(name, delete)(name * self) RBC_NONNULL;

#define RBC_ARRAY_DEF(type) RBC_ARRAY_DEF_GENERIC(RBC_ARRAY_TYPE_NAME(type), type, RBC_EXPORT)

#define RBC_ARRAY_IMPL_GENERIC(name, type)                 \
	RBC_NODISCARD name RBC_METHOD(name, new)(usize size) { \
		return (name){malloc(size * sizeof(type)), size};  \
	}                                                      \
                                                           \
	void RBC_METHOD(name, delete)(name * self) {           \
		free(self->data);                                  \
	}

#define RBC_ARRAY_IMPL(type) RBC_ARRAY_IMPL_GENERIC(RBC_ARRAY_TYPE_NAME(type), type)

RBC_ARRAY_DEF(u8)
