#include <gmock/gmock.h>
#include <siplasplas/variant/variant.hpp>
#include "test_util.hpp"
#include <unordered_map>
#include <stdexcept>

using namespace ::testing;

template<typename TestCase>
struct VariantTest : public Test
{};

TYPED_TEST_CASE_P(VariantTest);

TYPED_TEST_P(VariantTest, DefaultConstruction_Empty)
{
    test_case::Variant<TypeParam> v;

    EXPECT_TRUE(v.empty());
}

TYPED_TEST_P(VariantTest, ConstructionFromValue_NotEmpty)
{
    test_case::Variant<TypeParam> v{TypeParam::getValue()};

    EXPECT_FALSE(v.empty());
}

TYPED_TEST_P(VariantTest, MoveAssignmentOfValue_NotEmpty)
{
    test_case::Variant<TypeParam> v;

    ASSERT_NO_THROW(v = TypeParam::getValue());
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(ctti::unnamed_type_id<test_case::value_type<TypeParam>>(), v.tag());
}

TYPED_TEST_P(VariantTest, CopyAssignmentOfValue_NotEmpty)
{
    test_case::Variant<TypeParam> v;
    auto lvalue = TypeParam::getValue();

    ASSERT_NO_THROW(v = lvalue);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(ctti::unnamed_type_id<test_case::value_type<TypeParam>>(), v.tag());
}

TYPED_TEST_P(VariantTest, ValueDestructorCalled)
{
    using Variant = make_test_variant_t<test_case::Variant<TypeParam>>;

    {
        Variant v{lifetime_registered(TypeParam::getValue())};
    }

    EXPECT_TRUE(LifetimeRegistered<test_case::value_type<TypeParam>>::latestWasDestroyed());
}



REGISTER_TYPED_TEST_CASE_P(VariantTest,
                           DefaultConstruction_Empty, ConstructionFromValue_NotEmpty,
                           ValueDestructorCalled,
                           MoveAssignmentOfValue_NotEmpty, CopyAssignmentOfValue_NotEmpty);



/* =========================================================== */
/* >>>>>>>>>>              TEST SETUP              <<<<<<<<<<< */
/* =========================================================== */


using variants = TypeList<
    cpp::Variant<int, char, bool>,
    cpp::Variant<std::string, std::vector<int>, char, float>,
    cpp::Variant<cpp::Variant<int, std::string>, bool>
>;

using test_cases = detail::functor_t<TestCases<variants>, Types>; 

INSTANTIATE_TYPED_TEST_CASE_P(TestMultipleVariantInstances, VariantTest, 
    test_cases
);
