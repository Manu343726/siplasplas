#include <siplasplas/utility/typeinfo.hpp>
#include <siplasplas/utility/preprocessor.hpp>
#include <gmock/gmock.h>

#define TRAIT_TEST(trait) \
    EXPECT_EQ(        \
        cpp::TypeInfo::get<int>()(cpp::TypeTrait::trait), \
        std::trait<int>::value \
    ); \
    static_assert(cpp::TypeInfo::get<int>()(cpp::TypeTrait::trait) \
        == std::trait<int>::value, "trait test failed")

TEST(TypeInfoTest, typeTraits)
{
    TRAIT_TEST(is_void);
    TRAIT_TEST(is_null_pointer);
    TRAIT_TEST(is_integral);
    TRAIT_TEST(is_floating_point);
    TRAIT_TEST(is_array);
    TRAIT_TEST(is_enum);
    TRAIT_TEST(is_union);
    TRAIT_TEST(is_class);
    TRAIT_TEST(is_function);
    TRAIT_TEST(is_pointer);
    TRAIT_TEST(is_lvalue_reference);
    TRAIT_TEST(is_rvalue_reference);
    TRAIT_TEST(is_member_object_pointer);
    TRAIT_TEST(is_member_function_pointer);
    TRAIT_TEST(is_fundamental);
    TRAIT_TEST(is_arithmetic);
    TRAIT_TEST(is_scalar);
    TRAIT_TEST(is_object);
    TRAIT_TEST(is_compound);
    TRAIT_TEST(is_reference);
    TRAIT_TEST(is_member_pointer);
    TRAIT_TEST(is_const);
    TRAIT_TEST(is_volatile);
    TRAIT_TEST(is_trivial);
    TRAIT_TEST(is_trivially_copyable);
    TRAIT_TEST(is_standard_layout);
    TRAIT_TEST(is_pod);
    TRAIT_TEST(is_literal_type);
    TRAIT_TEST(is_empty);
    TRAIT_TEST(is_polymorphic);
    TRAIT_TEST(is_abstract);
    TRAIT_TEST(is_signed);
    TRAIT_TEST(is_unsigned);
    TRAIT_TEST(is_default_constructible);
    TRAIT_TEST(is_copy_constructible);
    TRAIT_TEST(is_move_constructible);
    TRAIT_TEST(is_copy_assignable);
    TRAIT_TEST(is_move_assignable);
    TRAIT_TEST(is_destructible);
    TRAIT_TEST(is_trivially_default_constructible);
    TRAIT_TEST(is_trivially_copy_constructible);
    TRAIT_TEST(is_trivially_move_constructible);
    TRAIT_TEST(is_trivially_copy_assignable);
    TRAIT_TEST(is_trivially_move_assignable);
    TRAIT_TEST(is_trivially_destructible);
    TRAIT_TEST(is_nothrow_default_constructible);
    TRAIT_TEST(is_nothrow_copy_constructible);
    TRAIT_TEST(is_nothrow_move_constructible);
    TRAIT_TEST(is_nothrow_copy_assignable);
    TRAIT_TEST(is_nothrow_move_assignable);
    TRAIT_TEST(is_nothrow_destructible);
    TRAIT_TEST(has_virtual_destructor);
}
