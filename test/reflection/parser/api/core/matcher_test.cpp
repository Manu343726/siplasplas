#include <siplasplas/reflection/parser/api/core/matchers.hpp>
#include <siplasplas/reflection/parser/api/core/clang/visitorinterface.hpp>
#include <siplasplas/utility/anyof.hpp>
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
    std::unordered_map<std::string, bool> classes = {
        {"Foo", false},
        {"FooTemplate", false},
        {"FooTemplateMember", false}
    };

    class MyCallback : public MatchCallback
    {
    public:
        MyCallback(std::unordered_map<std::string, bool>& classes) :
            classes(classes)
        {}

    private:
        void onMatch(const MatchCallback::MatchResult& result) override final
        {
            EXPECT_EQ(result.root().kind(), cpp::anyOf({CursorKind::Kind::ClassDecl, CursorKind::Kind::ClassTemplate}));

            for(const auto& method : result.cursors().getAll("methods"))
            {
                EXPECT_EQ(method.kind(), CursorKind::Kind::CXXMethod);
            }
            for(const auto& method : result.cursors().getAll("staticMethods"))
            {
                EXPECT_EQ(method.kind(), CursorKind::Kind::CXXMethod);
            }
            for(const auto& field : result.cursors().getAll("fields"))
            {
                EXPECT_EQ(field.kind(), CursorKind::Kind::FieldDecl);
            }
            for(const auto& field : result.cursors().getAll("staticFields"))
            {
                EXPECT_EQ(field.kind(), CursorKind::Kind::FieldDecl);
            }

            classes[result.root().spelling().str().str()] = true;
        }

    private:
        std::unordered_map<std::string, bool>& classes;
    };

    MatchFinder finder;
    MyCallback callback{classes};

    auto classContentsMatcher = anyOf(
        has(id("fields",        fieldDecl(                    isPublic()))),
        has(id("methods",       cxxMethod(unless(isStatic()), isPublic()))),
        has(id("staticMethods", cxxMethod(       isStatic(),  isPublic())))
    );

    auto classMatcher = classDecl(classContentsMatcher);
    auto classTemplateMatcher = classTemplate(classContentsMatcher);

    finder.addMatcher(classMatcher, callback);
    finder.addMatcher(classTemplateMatcher, callback);

    finder.matchAst(tu);

    for(const auto& keyValue : classes)
    {
        const std::string& className = keyValue.first;
        EXPECT_TRUE(keyValue.second) << "Expected class or class template \"" << className
            << "\" to have been visited";
    }
}
