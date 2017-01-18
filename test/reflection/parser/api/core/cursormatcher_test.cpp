#include <siplasplas/reflection/parser/api/core/cursormatcher.hpp>
#include <test-utils/asttest.hpp>
#include <gmock/gmock.h>

using namespace ::cpp::reflection::parser::api::core;
using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::testing;

class CursorMatcherTest : public cpp::test::AstTest, public Test
{
public:
    CursorMatcherTest() :
        tu{index.parse(AstTest::testFile("helloworld.cpp"), CompileOptions()
            .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
            .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
            .std("c++11")
        )}
    {}

protected:
    TranslationUnit tu;
};

TEST_F(CursorMatcherTest, TranslationUnitMatcher_matchesParsedTranslationUnit)
{
    CursorMatcher tuMatcher{CursorKind::Kind::TranslationUnit};
    EXPECT_TRUE(tuMatcher.matches(tu.cursor()));

    CursorMatcher namedTuMatcher{CursorKind::Kind::TranslationUnit, AstTest::testFile("helloworld.cpp")};
    EXPECT_TRUE(namedTuMatcher.matches(tu.cursor()));
}
