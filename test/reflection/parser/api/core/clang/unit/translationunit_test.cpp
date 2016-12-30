#include <siplasplas/reflection/parser/api/core/clang/translationunit.hpp>
#include <gmock/gmock.h>
#include "clangtest.hpp"

using namespace ::testing;
using namespace ::cpp::reflection::parser::api::core::clang;

class TranslationUnitTest : public Test, cpp::test::ClangTest
{
protected:
    TranslationUnitTest() :
        cpp::test::ClangTest{"translationunit_test.cpp"},
        tu{index.parse("translationunit_test.cpp", CompileOptions()
            .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
            .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
            .std("c++11")
        )}
    {}

    TranslationUnit tu;
};

TEST_F(TranslationUnitTest, cursorNotNull)
{
    EXPECT_FALSE(tu.cursor().isNull());
}

TEST_F(TranslationUnitTest, cursorPointsToTranslationUnit)
{
    EXPECT_EQ(tu, tu.cursor().translationUnit());
}

TEST_F(TranslationUnitTest, spelling_MatchesSourceFileName)
{
    EXPECT_STREQ("translationunit_test.cpp", tu.spelling().c_str());
}
