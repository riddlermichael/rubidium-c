#include <catch2/catch_test_macros.hpp>

#include <rbc/containers/vector.h>
#include <rbc/core/helpers.h>

namespace {

// we can't to include <core/traits/IsSame.hpp> and don't want to include <type_traits>
template <class T, class U>
struct IsSame {
	enum {
		value = false
	};
};

template <class T>
struct IsSame<T, T> {
	enum {
		value = true
	};
};

using Type = int;

RBC_VECTOR_DEF_GENERIC(Vec, Type, RBC_HIDDEN)
RBC_VECTOR_IMPL_GENERIC(Vec, Type) // NOLINT
RBC_VECTOR_DEF_REL_OPS_GENERIC(Vec, Type, RBC_HIDDEN)
RBC_VECTOR_IMPL_REL_OPS_GENERIC(Vec, Type) // NOLINT(readability-suspicious-call-argument)

class VecFixture {
public:
	VecFixture() noexcept
	    : v(Vec_new()) { // just for clarity
	}

	~VecFixture() {
		Vec_delete(&v);
	}

protected:
	Vec v;
};

} // namespace

TEST_CASE_METHOD(VecFixture, "types", "[vector]") {
	STATIC_REQUIRE(IsSame<Vec_value_t, Type>::value);
}

TEST_CASE_METHOD(VecFixture, "new", "[vector]") {
	REQUIRE(Vec_size(&v) == 0);
	REQUIRE(Vec_capacity(&v) == 0);
	REQUIRE(Vec_empty(&v));
}

TEST_CASE_METHOD(VecFixture, "with_size", "[vector]") {
	usize const size = 42;
	v = Vec_with_size(size);
	REQUIRE(Vec_size(&v) == size);
	REQUIRE(Vec_capacity(&v) >= Vec_size(&v));
	REQUIRE_FALSE(Vec_empty(&v));
	for (usize idx = 0; idx < size; ++idx) {
		REQUIRE(v.data[idx] == Type{});
	}
}

TEST_CASE_METHOD(VecFixture, "with_size_and_value", "[vector]") {
	usize const size = 42;
	auto const value = static_cast<Type>(0xCAFEBABE);
	v = Vec_with_size_and_value(size, value);
	REQUIRE(Vec_size(&v) == size);
	REQUIRE(Vec_capacity(&v) >= Vec_size(&v));
	REQUIRE_FALSE(Vec_empty(&v));
	for (usize idx = 0; idx < size; ++idx) {
		REQUIRE(v.data[idx] == value);
	}
}

TEST_CASE_METHOD(VecFixture, "from_empty_list", "[vector]") {
	v = Vec_from_list(0);
	REQUIRE(Vec_size(&v) == 0);
	REQUIRE(Vec_capacity(&v) == 0);
	REQUIRE(Vec_empty(&v));
}

TEST_CASE_METHOD(VecFixture, "from_list", "[vector]") {
	Type const values[] = {1, 2, 3};
	usize const size = RBC_ARRAY_SIZE(values);
	v = Vec_from_list(3, values[0], values[1], values[2]);
	REQUIRE(Vec_size(&v) == size);
	REQUIRE(Vec_capacity(&v) >= Vec_size(&v));
	REQUIRE_FALSE(Vec_empty(&v));
	for (usize idx = 0; idx < size; ++idx) {
		REQUIRE(v.data[idx] == values[idx]);
	}
}

TEST_CASE_METHOD(VecFixture, "from_list_macro", "[vector]") {
	v = RBC_VECTOR_FROM_LIST(Vec, 1, 2, 3);
	REQUIRE(Vec_size(&v) == 3);
	REQUIRE(Vec_capacity(&v) >= Vec_size(&v));
	REQUIRE_FALSE(Vec_empty(&v));
	REQUIRE(v.data[0] == 1);
	REQUIRE(v.data[1] == 2);
	REQUIRE(v.data[2] == 3);
}

TEST_CASE_METHOD(VecFixture, "at", "[vector]") {
	Type const values[] = {1, 2, 3};
	usize const size = RBC_ARRAY_SIZE(values);
	v = Vec_from_list(3, values[0], values[1], values[2]);
	for (usize idx = 0; idx < size; ++idx) {
		REQUIRE(Vec_at(&v, idx) == values[idx]);
	}
}

TEST_CASE_METHOD(VecFixture, "at_ref", "[vector]") {
	Type const value = 42;
	v = RBC_VECTOR_FROM_LIST(Vec, value);
	Type const new_value = 43;
	*Vec_at_ref(&v, 0) = new_value;
	REQUIRE(Vec_at(&v, 0) == new_value);
}

TEST_CASE_METHOD(VecFixture, "data", "[vector]") {
	usize const size = 42;
	v = Vec_with_size(size);
	REQUIRE(Vec_data(&v) == v.data);
	REQUIRE(Vec_data(&v) == Vec_at_ref(&v, 0));
	REQUIRE(Vec_data_const(&v) == Vec_at_ref(&v, 0));
}

TEST_CASE_METHOD(VecFixture, "clear", "[vector]") {
	usize const size = 42;
	v = Vec_with_size(size);
	usize const capacity = Vec_capacity(&v);
	Vec_clear(&v);
	REQUIRE(Vec_size(&v) == 0);
	REQUIRE(Vec_empty(&v));
	REQUIRE(Vec_capacity(&v) == capacity);
}

TEST_CASE_METHOD(VecFixture, "swap", "[vector]") {
	usize const v_size = 42;
	v = Vec_with_size(v_size);
	usize const v_cap = Vec_capacity(&v);
	Type const* v_data = Vec_data(&v);

	usize const u_size = 17;
	Vec u = Vec_with_size(u_size);
	usize const u_cap = Vec_capacity(&u);
	Type const* u_data = Vec_data(&u);

	Vec_swap(&v, &u);
	REQUIRE(Vec_size(&v) == u_size);
	REQUIRE(Vec_size(&u) == v_size);
	REQUIRE(Vec_capacity(&v) == u_cap);
	REQUIRE(Vec_capacity(&u) == v_cap);
	REQUIRE(Vec_data(&v) == u_data);
	REQUIRE(Vec_data(&u) == v_data);
}

TEST_CASE_METHOD(VecFixture, "eq_empty", "[vector]") {
	REQUIRE(Vec_eq(&v, &v));
}

TEST_CASE_METHOD(VecFixture, "eq", "[vector]") {
	usize const v_size = 42;
	v = Vec_with_size(v_size);
	REQUIRE(Vec_eq(&v, &v));
}
