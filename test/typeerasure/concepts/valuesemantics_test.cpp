#include "../mocks/valuesemantics.hpp"
#include <siplasplas/typeerasure/concepts/valuesemantics.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp;


TEST(ConceptsValueSemanticsTest, defaultConstructibleType_isDefaultConstructible)
{
    EXPECT_TRUE(concepts::DefaultConstructible<DefaultConstructible>::value);
    EXPECT_FALSE(concepts::DefaultConstructible<DefaultConstructible>::no_except);
    EXPECT_TRUE(concepts::DefaultConstructible<NoThrowDefaultConstructible>::value);
    EXPECT_TRUE(concepts::DefaultConstructible<NoThrowDefaultConstructible>::no_except);
}

TEST(ConceptsValueSemanticsTest, noDefaultConstructibleType_isNotDefaultConstructible)
{
    EXPECT_FALSE(concepts::DefaultConstructible<NoDefaultConstructible>::value);
    EXPECT_FALSE(concepts::DefaultConstructible<NoDefaultConstructible>::no_except);
    EXPECT_FALSE(concepts::DefaultConstructible<NoNoThrowDefaultConstructible>::value);
    EXPECT_FALSE(concepts::DefaultConstructible<NoNoThrowDefaultConstructible>::no_except);
}

TEST(ConceptsValueSemanticsTest, constructibleType_isConstructible)
{
    EXPECT_TRUE(concepts::Constructible<Constructible>::value);
    EXPECT_FALSE(concepts::Constructible<Constructible>::no_except);
    EXPECT_TRUE(concepts::Constructible<NoThrowConstructible>::value);
    EXPECT_TRUE(concepts::Constructible<NoThrowConstructible>::no_except);
    EXPECT_TRUE((concepts::Constructible<Constructible, int>::value));
    EXPECT_FALSE((concepts::Constructible<Constructible, int>::no_except));
    EXPECT_TRUE((concepts::Constructible<NoThrowConstructible, int>::value));
    EXPECT_TRUE((concepts::Constructible<NoThrowConstructible, int>::no_except));
}

TEST(ConceptsValueSemanticsTest, noConstructibleType_isNotConstructible)
{
    EXPECT_FALSE(concepts::Constructible<NoConstructible>::value);
    EXPECT_FALSE(concepts::Constructible<NoConstructible>::no_except);
    EXPECT_FALSE(concepts::Constructible<NoNoThrowConstructible>::value);
    EXPECT_FALSE(concepts::Constructible<NoNoThrowConstructible>::no_except);
    EXPECT_FALSE((concepts::Constructible<NoConstructible, int>::value));
    EXPECT_FALSE((concepts::Constructible<NoConstructible, int>::no_except));
    EXPECT_FALSE((concepts::Constructible<NoNoThrowConstructible, int>::value));
    EXPECT_FALSE((concepts::Constructible<NoNoThrowConstructible, int>::no_except));
}

TEST(ConceptsValueSemanticsTest, copyConstructibleType_isCopyConstructible)
{
    EXPECT_TRUE(concepts::CopyConstructible<CopyConstructible>::value);
    EXPECT_FALSE(concepts::CopyConstructible<CopyConstructible>::no_except);
    EXPECT_TRUE(concepts::CopyConstructible<NoThrowCopyConstructible>::value);
    EXPECT_TRUE(concepts::CopyConstructible<NoThrowCopyConstructible>::no_except);
}

TEST(ConceptsValueSemanticsTest, noCopyConstructibleType_isNotCopyConstructible)
{
    EXPECT_FALSE(concepts::CopyConstructible<NoCopyConstructible>::value);
    EXPECT_FALSE(concepts::CopyConstructible<NoCopyConstructible>::no_except);
    EXPECT_FALSE(concepts::CopyConstructible<NoNoThrowCopyConstructible>::value);
    EXPECT_FALSE(concepts::CopyConstructible<NoNoThrowCopyConstructible>::no_except);
}

TEST(ConceptsValueSemanticsTest, moveConstructibleType_isMoveConstructible)
{
    EXPECT_TRUE(concepts::MoveConstructible<MoveConstructible>::value);
    EXPECT_FALSE(concepts::MoveConstructible<MoveConstructible>::no_except);
    EXPECT_TRUE(concepts::MoveConstructible<NoThrowMoveConstructible>::value);
    EXPECT_TRUE(concepts::MoveConstructible<NoThrowMoveConstructible>::no_except);
}

TEST(ConceptsValueSemanticsTest, noMoveConstructibleType_isNotMoveConstructible)
{
    EXPECT_FALSE(concepts::MoveConstructible<NoMoveConstructible>::value);
    EXPECT_FALSE(concepts::MoveConstructible<NoMoveConstructible>::no_except);
    EXPECT_FALSE(concepts::MoveConstructible<NoNoThrowMoveConstructible>::value);
    EXPECT_FALSE(concepts::MoveConstructible<NoNoThrowMoveConstructible>::no_except);
}

TEST(ConceptsValueSemanticsTest, assignableType_isAssignable)
{
    EXPECT_TRUE((concepts::Assignable<Assignable, int>::value));
    EXPECT_FALSE((concepts::Assignable<Assignable, int>::no_except));
    EXPECT_TRUE((concepts::Assignable<NoThrowAssignable, int>::value));
    EXPECT_TRUE((concepts::Assignable<NoThrowAssignable, int>::no_except));
}

TEST(ConceptsValueSemanticsTest, noAssignableType_isNotAssignable)
{
    EXPECT_FALSE((concepts::Assignable<NoAssignable, int>::value));
    EXPECT_FALSE((concepts::Assignable<NoAssignable, int>::no_except));
    EXPECT_FALSE((concepts::Assignable<NoNoThrowAssignable, int>::value));
    EXPECT_FALSE((concepts::Assignable<NoNoThrowAssignable, int>::no_except));
}

TEST(ConceptsValueSemanticsTest, copyAssignableType_isCopyAssignable)
{
    EXPECT_TRUE(concepts::CopyAssignable<CopyAssignable>::value);
    EXPECT_FALSE(concepts::CopyAssignable<CopyAssignable>::no_except);
    EXPECT_TRUE(concepts::CopyAssignable<NoThrowCopyAssignable>::value);
    EXPECT_TRUE(concepts::CopyAssignable<NoThrowCopyAssignable>::no_except);
}

TEST(ConceptsValueSemanticsTest, noCopyAssignableType_isNotCopyAssignable)
{
    EXPECT_FALSE(concepts::CopyAssignable<NoCopyAssignable>::value);
    EXPECT_FALSE(concepts::CopyAssignable<NoCopyAssignable>::no_except);
    EXPECT_FALSE(concepts::CopyAssignable<NoNoThrowCopyAssignable>::value);
    EXPECT_FALSE(concepts::CopyAssignable<NoNoThrowCopyAssignable>::no_except);
}

TEST(ConceptsValueSemanticsTest, moveAssignableType_isMoveAssignable)
{
    EXPECT_TRUE(concepts::MoveAssignable<MoveAssignable>::value);
    EXPECT_FALSE(concepts::MoveAssignable<MoveAssignable>::no_except);
    EXPECT_TRUE(concepts::MoveAssignable<NoThrowMoveAssignable>::value);
    EXPECT_TRUE(concepts::MoveAssignable<NoThrowMoveAssignable>::no_except);
}

TEST(ConceptsValueSemanticsTest, noMoveAssignableType_isNotMoveAssignable)
{
    EXPECT_FALSE(concepts::MoveAssignable<NoMoveAssignable>::value);
    EXPECT_FALSE(concepts::MoveAssignable<NoMoveAssignable>::no_except);
    EXPECT_FALSE(concepts::MoveAssignable<NoNoThrowMoveAssignable>::value);
    EXPECT_FALSE(concepts::MoveAssignable<NoNoThrowMoveAssignable>::no_except);
}

TEST(ConceptsValueSemanticsTest, destructibleType_isDestructible)
{
    EXPECT_TRUE(concepts::Destructible<Destructible>::value);
    EXPECT_FALSE(concepts::Destructible<Destructible>::no_except);
    EXPECT_TRUE(concepts::Destructible<NoThrowDestructible>::value);
    EXPECT_TRUE(concepts::Destructible<NoThrowDestructible>::no_except);
}

TEST(ConceptsValueSemanticsTest, noDestructibleType_isNotDestructible)
{
    EXPECT_FALSE(concepts::Destructible<NoDestructible>::value);
    EXPECT_FALSE(concepts::Destructible<NoDestructible>::no_except);
    EXPECT_FALSE(concepts::Destructible<NoNoThrowDestructible>::value);
    EXPECT_FALSE(concepts::Destructible<NoNoThrowDestructible>::no_except);
}
