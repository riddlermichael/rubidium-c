#pragma once

#include <stdarg.h>
#include <stdlib.h>

#include <rbc/containers/macros.h>
#include <rbc/core/assert.h>
#include <rbc/core/attributes.h>
#include <rbc/core/helpers.h>
#include <rbc/core/types.h>
#include <rbc/utils/swap.h>

// def
#define RBC_VECTOR_TYPE_NAME(type) rbc_vector_##type

#define RBC_VECTOR_DEF_GENERIC(name, type, export)                                                                     \
	typedef type RBC_CONCAT(name, _value_t);                                                                           \
	typedef int (*RBC_CONCAT(name, _cmp_fn))(type lhs, type rhs);                                                      \
                                                                                                                       \
	struct name {                                                                                                      \
		type* data;                                                                                                    \
		usize size;                                                                                                    \
		usize cap;                                                                                                     \
	};                                                                                                                 \
	typedef struct name name;                                                                                          \
                                                                                                                       \
	RBC_BEGIN_EXTERN_C                                                                                                 \
	export RBC_NODISCARD name RBC_METHOD(name, new)(void);                                                             \
	export RBC_NODISCARD name RBC_METHOD(name, with_size)(usize size);                                                 \
	export RBC_NODISCARD name RBC_METHOD(name, with_size_and_value)(usize size, type value);                           \
	export RBC_NODISCARD name RBC_METHOD(name, from)(type const* begin, type const* end) RBC_NONNULL;                  \
	export RBC_NODISCARD name RBC_METHOD(name, from_list)(usize size, ...);                                            \
	export RBC_NODISCARD name RBC_METHOD(name, copy)(name const* rhs) RBC_NONNULL;                                     \
	export RBC_NODISCARD name RBC_METHOD(name, move)(name * rhs) RBC_NONNULL;                                          \
	export void RBC_METHOD(name, delete)(name * self) RBC_NONNULL;                                                     \
	export void RBC_METHOD(name, copy_assign)(name const* rhs) RBC_NONNULL;                                            \
	export void RBC_METHOD(name, move_assign)(name * rhs) RBC_NONNULL;                                                 \
	export type RBC_METHOD(name, at)(name const* self, usize pos) RBC_NONNULL;                                         \
	export type* RBC_METHOD(name, at_ref)(name * self, usize pos) RBC_NONNULL;                                         \
	export type RBC_METHOD(name, front)(name const* self) RBC_NONNULL;                                                 \
	export type* RBC_METHOD(name, front_ref)(name * self) RBC_NONNULL;                                                 \
	export type RBC_METHOD(name, back)(name const* self) RBC_NONNULL;                                                  \
	export type* RBC_METHOD(name, back_ref)(name * self) RBC_NONNULL;                                                  \
	export type* RBC_METHOD(name, data)(name * self) RBC_NONNULL;                                                      \
	export type const* RBC_METHOD(name, data_const)(name const* self) RBC_NONNULL;                                     \
	export usize RBC_METHOD(name, size)(name const* self) RBC_NONNULL;                                                 \
	export usize RBC_METHOD(name, capacity)(name const* self) RBC_NONNULL;                                             \
	export bool RBC_METHOD(name, empty)(name const* self) RBC_NONNULL;                                                 \
	export void RBC_METHOD(name, reserve)(name * self, usize new_capacity) RBC_NONNULL;                                \
	export void RBC_METHOD(name, shrink_to_fit)(name * self) RBC_NONNULL;                                              \
	export void RBC_METHOD(name, clear)(name * self) RBC_NONNULL;                                                      \
	export void RBC_METHOD(name, push_back)(name * self, type value) RBC_NONNULL_ARGS(1);                              \
	export void RBC_METHOD(name, pop_back)(name * self) RBC_NONNULL;                                                   \
	export void RBC_METHOD(name, resize)(name * self, usize count) RBC_NONNULL;                                        \
	export void RBC_METHOD(name, resize_with_value)(name * self, usize count, type value) RBC_NONNULL_ARGS(1);         \
	export void RBC_METHOD(name, swap)(name * lhs, name * rhs) RBC_NONNULL;                                            \
	export usize RBC_METHOD(name, erase_value)(name * self, type value) RBC_NONNULL_ARGS(1);                           \
	export usize RBC_METHOD(name, erase_if)(name * self, bool (*pred)(type)) RBC_NONNULL;                              \
	export bool RBC_METHOD(name, eq_cmp)(name const* lhs, name const* rhs, RBC_CONCAT(name, _cmp_fn) cmp) RBC_NONNULL; \
	export bool RBC_METHOD(name, ne_cmp)(name const* lhs, name const* rhs, RBC_CONCAT(name, _cmp_fn) cmp) RBC_NONNULL; \
	export bool RBC_METHOD(name, lt_cmp)(name const* lhs, name const* rhs, RBC_CONCAT(name, _cmp_fn) cmp) RBC_NONNULL; \
	export bool RBC_METHOD(name, gt_cmp)(name const* lhs, name const* rhs, RBC_CONCAT(name, _cmp_fn) cmp) RBC_NONNULL; \
	export bool RBC_METHOD(name, le_cmp)(name const* lhs, name const* rhs, RBC_CONCAT(name, _cmp_fn) cmp) RBC_NONNULL; \
	export bool RBC_METHOD(name, ge_cmp)(name const* lhs, name const* rhs, RBC_CONCAT(name, _cmp_fn) cmp) RBC_NONNULL; \
	RBC_END_EXTERN_C

#define RBC_VECTOR_DEF(type) RBC_VECTOR_DEF_GENERIC(RBC_VECTOR_TYPE_NAME(type), type, RBC_EXPORT)

// impl
#define RBC_VECTOR_IMPL_GENERIC(name, type)                                            \
	RBC_NODISCARD name RBC_METHOD(name, new)(void) {                                   \
		name self;                                                                     \
		self.data = NULL;                                                              \
		self.size = 0;                                                                 \
		self.cap = 0;                                                                  \
		return self;                                                                   \
	}                                                                                  \
                                                                                       \
	RBC_NODISCARD name RBC_METHOD(name, with_size)(usize size) {                       \
		name self;                                                                     \
		self.data = size ? (type*) calloc(size, sizeof(type)) : NULL;                  \
		self.size = size;                                                              \
		self.cap = size;                                                               \
		return self;                                                                   \
	}                                                                                  \
                                                                                       \
	RBC_NODISCARD name RBC_METHOD(name, with_size_and_value)(usize size, type value) { \
		name self = RBC_METHOD(name, with_size)(size);                                 \
		for (usize idx = 0; idx < size; ++idx) {                                       \
			self.data[idx] = value;                                                    \
		}                                                                              \
		return self;                                                                   \
	}                                                                                  \
                                                                                       \
	RBC_NODISCARD name RBC_METHOD(name, from)(type const* begin, type const* end) {    \
		isize const sz = end - begin;                                                  \
		if (sz < 0) {                                                                  \
			return RBC_METHOD(name, new)();                                            \
		}                                                                              \
                                                                                       \
		usize const size = (usize) sz;                                                 \
		name self = RBC_METHOD(name, with_size)(size);                                 \
		for (usize idx = 0; idx < size; ++idx) {                                       \
			self.data[idx] = begin[idx];                                               \
		}                                                                              \
		return self;                                                                   \
	}                                                                                  \
                                                                                       \
	RBC_NODISCARD name RBC_METHOD(name, from_list)(usize size, ...) {                  \
		name self = RBC_METHOD(name, with_size)(size);                                 \
		va_list args;                                                                  \
		va_start(args, size);                                                          \
		for (usize idx = 0; idx < size; ++idx) {                                       \
			self.data[idx] = va_arg(args, type);                                       \
		}                                                                              \
		va_end(args);                                                                  \
		return self;                                                                   \
	}                                                                                  \
                                                                                       \
	RBC_NODISCARD name RBC_METHOD(name, copy)(name const* rhs) {                       \
		name self = RBC_METHOD(name, with_size)(rhs->size);                            \
		for (usize idx = 0; idx < self.size; ++idx) {                                  \
			self.data[idx] = rhs->data[idx];                                           \
		}                                                                              \
		return self;                                                                   \
	}                                                                                  \
                                                                                       \
	RBC_NODISCARD name RBC_METHOD(name, move)(name * rhs) {                            \
		name self = *rhs;                                                              \
		rhs->data = NULL;                                                              \
		rhs->size = 0;                                                                 \
		rhs->cap = 0;                                                                  \
		return self;                                                                   \
	}                                                                                  \
                                                                                       \
	void RBC_METHOD(name, delete)(name * self) {                                       \
		free(self->data);                                                              \
		self->data = NULL;                                                             \
		self->size = 0;                                                                \
	}                                                                                  \
                                                                                       \
	type RBC_METHOD(name, at)(name const* self, usize pos) {                           \
		RBC_ASSERT_X(pos < self->size, "Index %zu is out of range", pos);              \
		return self->data[pos];                                                        \
	}                                                                                  \
                                                                                       \
	type* RBC_METHOD(name, at_ref)(name * self, usize pos) {                           \
		RBC_ASSERT_X(pos < self->size, "Index %zu is out of range", pos);              \
		return &self->data[pos];                                                       \
	}                                                                                  \
                                                                                       \
	type RBC_METHOD(name, front)(name const* self) {                                   \
		return RBC_METHOD(name, at)(self, 0);                                          \
	}                                                                                  \
                                                                                       \
	type* RBC_METHOD(name, front_ref)(name * self) {                                   \
		return RBC_METHOD(name, at_ref)(self, 0);                                      \
	}                                                                                  \
                                                                                       \
	type RBC_METHOD(name, back)(name const* self) {                                    \
		return RBC_METHOD(name, at)(self, self->size - 1);                             \
	}                                                                                  \
                                                                                       \
	type* RBC_METHOD(name, back_ref)(name * self) {                                    \
		return RBC_METHOD(name, at_ref)(self, self->size - 1);                         \
	}                                                                                  \
                                                                                       \
	type* RBC_METHOD(name, data)(name * self) {                                        \
		return self->data;                                                             \
	}                                                                                  \
                                                                                       \
	type const* RBC_METHOD(name, data_const)(name const* self) {                       \
		return self->data;                                                             \
	}                                                                                  \
                                                                                       \
	usize RBC_METHOD(name, size)(name const* self) {                                   \
		return self->size;                                                             \
	}                                                                                  \
                                                                                       \
	usize RBC_METHOD(name, capacity)(name const* self) {                               \
		return self->cap;                                                              \
	}                                                                                  \
                                                                                       \
	bool RBC_METHOD(name, empty)(name const* self) {                                   \
		return self->size == 0;                                                        \
	}                                                                                  \
                                                                                       \
	void RBC_METHOD(name, clear)(name * self) {                                        \
		free(self->data);                                                              \
		self->data = NULL;                                                             \
		self->size = 0;                                                                \
	}                                                                                  \
                                                                                       \
	void RBC_METHOD(name, push_back)(name * self, type value) {                        \
		RBC_UNUSED(self);                                                              \
		RBC_UNUSED(value);                                                             \
		RBC_ASSERT_X(false, "Not implemented yet");                                    \
	}                                                                                  \
                                                                                       \
	void RBC_METHOD(name, pop_back)(name * self) {                                     \
		RBC_UNUSED(self);                                                              \
		RBC_ASSERT_X(false, "Not implemented yet");                                    \
	}                                                                                  \
                                                                                       \
	void RBC_METHOD(name, swap)(name * lhs, name * rhs) {                              \
		RBC_SWAP(lhs->data, rhs->data);                                                \
		RBC_SWAP(lhs->size, rhs->size);                                                \
		RBC_SWAP(lhs->cap, rhs->cap);                                                  \
	}                                                                                  \
                                                                                       \
	bool RBC_METHOD(name, eq_cmp)(name const* lhs, name const* rhs,                    \
	    RBC_CONCAT(name, _cmp_fn) cmp) {                                               \
		if (lhs->size != rhs->size) {                                                  \
			return false;                                                              \
		}                                                                              \
		for (usize idx = 0; idx < lhs->size; ++idx) {                                  \
			if (cmp(lhs->data[idx], rhs->data[idx])) {                                 \
				return false;                                                          \
			}                                                                          \
		}                                                                              \
		return true;                                                                   \
	}                                                                                  \
                                                                                       \
	bool RBC_METHOD(name, ne_cmp)(name const* lhs, name const* rhs,                    \
	    RBC_CONCAT(name, _cmp_fn) cmp) {                                               \
		return !RBC_METHOD(name, eq_cmp)(lhs, rhs, cmp);                               \
	}                                                                                  \
                                                                                       \
	bool RBC_METHOD(name, lt_cmp)(name const* lhs, name const* rhs,                    \
	    RBC_CONCAT(name, _cmp_fn) cmp) {                                               \
		type const* lhs_first = lhs->data;                                             \
		type const* rhs_first = rhs->data;                                             \
		type const* lhs_last = lhs->data + lhs->size;                                  \
		type const* rhs_last = rhs->data + rhs->size;                                  \
		for (; (lhs_first != lhs_last) && (rhs_first != rhs_last);                     \
		    ++lhs_first, ++rhs_first) {                                                \
			if (cmp(*lhs_first, *rhs_first) < 0) {                                     \
				return true;                                                           \
			}                                                                          \
			if (cmp(*rhs_first, *lhs_first) < 0) {                                     \
				return false;                                                          \
			}                                                                          \
		}                                                                              \
		return (lhs_first == lhs_last) && (rhs_first != rhs_last);                     \
	}                                                                                  \
                                                                                       \
	bool RBC_METHOD(name, gt_cmp)(name const* lhs, name const* rhs,                    \
	    RBC_CONCAT(name, _cmp_fn) cmp) {                                               \
		return RBC_METHOD(name, lt_cmp)(rhs, lhs, cmp);                                \
	}                                                                                  \
                                                                                       \
	bool RBC_METHOD(name, le_cmp)(name const* lhs, name const* rhs,                    \
	    RBC_CONCAT(name, _cmp_fn) cmp) {                                               \
		return !RBC_METHOD(name, lt_cmp)(rhs, lhs, cmp);                               \
	}                                                                                  \
                                                                                       \
	bool RBC_METHOD(name, ge_cmp)(name const* lhs, name const* rhs,                    \
	    RBC_CONCAT(name, _cmp_fn) cmp) {                                               \
		return !RBC_METHOD(name, lt_cmp)(lhs, rhs, cmp);                               \
	}

#define RBC_VECTOR_IMPL(type) RBC_VECTOR_IMPL_GENERIC(RBC_VECTOR_TYPE_NAME(type), type)

// rel ops
#define RBC_VECTOR_DEF_REL_OPS_GENERIC(name, type, export)                          \
	RBC_BEGIN_EXTERN_C                                                              \
	export bool RBC_METHOD(name, eq)(name const* lhs, name const* rhs) RBC_NONNULL; \
	export bool RBC_METHOD(name, ne)(name const* lhs, name const* rhs) RBC_NONNULL; \
	export bool RBC_METHOD(name, lt)(name const* lhs, name const* rhs) RBC_NONNULL; \
	export bool RBC_METHOD(name, le)(name const* lhs, name const* rhs) RBC_NONNULL; \
	export bool RBC_METHOD(name, gt)(name const* lhs, name const* rhs) RBC_NONNULL; \
	export bool RBC_METHOD(name, ge)(name const* lhs, name const* rhs) RBC_NONNULL; \
	RBC_END_EXTERN_C

#define RBC_VECTOR_DEF_REL_OPS(type) RBC_VECTOR_DEF_REL_OPS_GENERIC(RBC_VECTOR_TYPE_NAME(type), type, RBC_EXPORT)

#define RBC_VECTOR_IMPL_REL_OPS_GENERIC(name, type)                \
	bool RBC_METHOD(name, eq)(name const* lhs, name const* rhs) {  \
		if (lhs->size != rhs->size) {                              \
			return false;                                          \
		}                                                          \
		for (usize idx = 0; idx < lhs->size; ++idx) {              \
			if (lhs->data[idx] != rhs->data[idx]) {                \
				return false;                                      \
			}                                                      \
		}                                                          \
		return true;                                               \
	}                                                              \
                                                                   \
	bool RBC_METHOD(name, ne)(name const* lhs, name const* rhs) {  \
		return !RBC_METHOD(name, eq)(lhs, rhs);                    \
	}                                                              \
                                                                   \
	bool RBC_METHOD(name, lt)(name const* lhs, name const* rhs) {  \
		type const* lhs_first = lhs->data;                         \
		type const* rhs_first = rhs->data;                         \
		type const* lhs_last = lhs->data + lhs->size;              \
		type const* rhs_last = rhs->data + rhs->size;              \
		for (; (lhs_first != lhs_last) && (rhs_first != rhs_last); \
		    ++lhs_first, ++rhs_first) {                            \
			if (*lhs_first < *rhs_first) {                         \
				return true;                                       \
			}                                                      \
			if (*rhs_first < *lhs_first) {                         \
				return false;                                      \
			}                                                      \
		}                                                          \
		return (lhs_first == lhs_last) && (rhs_first != rhs_last); \
	}                                                              \
                                                                   \
	bool RBC_METHOD(name, gt)(name const* lhs, name const* rhs) {  \
		return RBC_METHOD(name, lt)(rhs, lhs);                     \
	}                                                              \
                                                                   \
	bool RBC_METHOD(name, le)(name const* lhs, name const* rhs) {  \
		return !RBC_METHOD(name, lt)(rhs, lhs);                    \
	}                                                              \
                                                                   \
	bool RBC_METHOD(name, ge)(name const* lhs, name const* rhs) {  \
		return !RBC_METHOD(name, lt)(lhs, rhs);                    \
	}

#define RBC_VECTOR_IMPL_REL_OPS(type) RBC_VECTOR_IMPL_REL_OPS_GENERIC(RBC_VECTOR_TYPE_NAME(type), type)

// macros
#define RBC_VECTOR_FROM_LIST(V, ...) RBC_METHOD(V, from_list)(RBC_NARGS(__VA_ARGS__), __VA_ARGS__)
