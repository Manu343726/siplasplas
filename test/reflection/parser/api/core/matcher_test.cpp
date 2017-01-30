#include <siplasplas/reflection/parser/api/core/matchers.hpp>
#include <siplasplas/reflection/parser/api/core/clang/visitorinterface.hpp>
#include <test-utils/asttest.hpp>
#include <gmock/gmock.h>
#include <iostream>

using namespace ::cpp::reflection::parser::api::core;
using namespace ::cpp::reflection::parser::api::core::matchers;
using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::testing;

class CursorMatcherTest : public cpp::test::AstTest, public Test
{
public:
    CursorMatcherTest() :
        tu{index.parse(AstTest::testFile("simpleclasses.hpp"), CompileOptions()
            .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
            .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
            .std("c++11")
        )}
    {}

    void SetUp()
    {
        ASSERT_FALSE(tu.isNull());
    }

protected:
    TranslationUnit tu;
};

TEST_F(CursorMatcherTest, classDeclMatcher_matchessClassesWithSpecificNameOnly)
{
    class MyCallback : public MatchCallback
    {
    private:
        void onMatch(const MatchCallback::MatchResult& result) override final
        {
            std::cout << "On " << result.root() << std::endl;

            EXPECT_EQ(CursorKind::Kind::Namespace, result.root().kind());
            //EXPECT_EQ(CursorKind::Kind::ClassDecl, result.cursors().get("class").kind());
        }
    };

    MatchFinder finder;
    MyCallback callback;

    finder.addMatcher(
        namespaceDecl(unless(has(classDecl()))),
        callback
    );

    finder.matchAst(tu);
}
