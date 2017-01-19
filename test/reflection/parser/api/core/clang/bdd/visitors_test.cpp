#include <catch.hpp>
#include <test-utils/asttest.hpp>
#include <siplasplas/reflection/parser/api/core/clang/visitor.hpp>
#include <siplasplas/reflection/parser/api/core/clang/visitorinterface.hpp>
#include <siplasplas/reflection/parser/api/core/clang/recursivevisitor.hpp>
#include <siplasplas/reflection/parser/api/core/clang/kindvisitor.hpp>
#include <iostream>

using namespace ::cpp::reflection::parser::api::core::clang;

class CounterVisitor : public Visitor
{
public:
    bool visit(const Cursor& root) const
    {
        _count = 0;
        return Visitor::visit(root);
    }

    bool visit(const Cursor& root)
    {
        _count = 0;
        return Visitor::visit(root);
    }

    Visitor::Result onCursor(visitor_tags::Simple tag, const Cursor& current, const Cursor& parent) const override
    {
        _count++;
        std::cout << current << std::endl;
        return Visitor::onCursor(tag, current, parent);
    }

    Visitor::Result onCursor(visitor_tags::Simple tag, const Cursor& current, const Cursor& parent) override
    {
        _count++;
        std::cout << current << std::endl;
        return Visitor::onCursor(tag, current, parent);
    }

    std::size_t count() const
    {
        return _count;
    }

private:
    mutable std::size_t _count = 0;
};

SCENARIO("Visitors abort AST traversal by default")
{
    GIVEN("An index and a C++11 sourcefile to parse")
    {
        Index index;
        REQUIRE(!index.isNull());

        WHEN("The sourcefile is parsed with clang include dirs and C++11 option")
        {
            TranslationUnit tu = index.parse(cpp::test::AstTest::testFile("helloworld.cpp"), CompileOptions()
                .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
                .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
                .std("c++11")
            );

            REQUIRE(!tu.isNull());

            THEN("The visitation of the translation unit is aborted inmediately using default visitors")
            {
                CounterVisitor visitor;
                const CounterVisitor constVisitor;

                REQUIRE(tu.cursor().kind() == CursorKind::Kind::TranslationUnit);
                REQUIRE(tu.cursor().kind().str().str() == "TranslationUnit");

                REQUIRE(visitor.visit(tu.cursor()));
                REQUIRE(visitor.count() == 1);
                REQUIRE(constVisitor.visit(tu.cursor()));
                REQUIRE(constVisitor.count() == 1);
            }
        }
    }
}

SCENARIO("Kind visitors visit nodes of a specific kind only")
{
    GIVEN("An index and a C++11 sourcefile to parse")
    {
        Index index;
        cpp::test::AstTest::writeFile("visitors_test.hpp",
        {
"#include <chrono>",
"",
"namespace foo",
"{",
"    class Foo {};",
"    namespace bar {}",
"    namespace quux {};",
"    namespace foobar",
"    {",
"        namespace foobarquux {}",
"    }",
"}"});

        REQUIRE(!index.isNull());

        WHEN("The sourcefile is parsed with clang include dirs and C++11 option")
        {
            TranslationUnit tu = index.parse("visitors_test.hpp", CompileOptions()
                .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
                .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
                .std("c++11")
            );

            REQUIRE(!tu.isNull());

            THEN("A namespace kind visitor visits namespaces only")
            {
                std::unordered_map<std::string, std::size_t> namespaces = {
                    { "foo", 0 },
                    { "bar", 0 },
                    { "quux", 0 },
                    { "foobar", 0 },
                    { "foobarquux", 0 },
                };

                class NamespaceVisitor : public VisitorInterface::Make<KindVisitor<
                    CursorKind::Kind::Namespace,
                    RecursiveVisitor
                >>
                {
                public:
                    NamespaceVisitor(std::unordered_map<std::string, std::size_t>& namespaces) :
                        namespaces(namespaces)
                    {}

                    VisitorResult onCursor(const Cursor& current, const Cursor& parent) override
                    {
                        REQUIRE(current.kind() == CursorKind::Kind::Namespace);

                        auto it = namespaces.find(current.spelling().str().str());

                        if(it != namespaces.end())
                        {
                            it->second++;
                            std::cout << "On " << current << " (Parent: " << parent << ") [count=" << it->second << "]" << std::endl;
                        }
                        else
                        {
                            std::cout << "On " << current << " (Parent: " << parent << ")" << std::endl;
                        }

                        return VisitorResult::Continue;
                    }

                private:
                    std::unordered_map<std::string, std::size_t>& namespaces;
                };

                NamespaceVisitor visitor{namespaces};
                visitor.visit(tu.cursor());

                THEN("All namespaces are visited at least once")
                {
                    for(const auto& keyValue : namespaces)
                    {
                        const auto& namespace_ = keyValue.first;
                        const auto& visited    = keyValue.second;

                        REQUIRE(visited == 1);
                    }
                }
            }
        }
    }
}
