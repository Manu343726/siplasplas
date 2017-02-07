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

            EXPECT_EQ(CursorKind::Kind::ClassDecl, result.root().kind());

            for(const auto& method : result.cursors().getAll("methods"))
            {
                std::cout << "Class " << result.root().spelling()
                    << ", method " << method.displayName() << std::endl;
            }
            for(const auto& method : result.cursors().getAll("staticMethods"))
            {
                std::cout << "Class " << result.root().spelling()
                    << ", static method " << method.displayName() << std::endl;
            }
        }
    };

    MatchFinder finder;
    MyCallback callback;

    finder.addMatcher(
        classDecl(
            has(anyOf(
                id("methods", cxxMethod(unless(isStatic()))),
                id("staticMethods", cxxMethod(isStatic()))
            ))
        ),
        callback
    );

    finder.matchAst(tu);
}
