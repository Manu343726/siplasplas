#include <catch.hpp>
#include <test-utils/asttest.hpp>

using namespace ::cpp::reflection::parser::api::core::clang;

SCENARIO("A C++11 hello world succesful compilation doesn't generate diagnostics")
{
    GIVEN("An index and a sourcefile to parse")
    {
        Index index;
        REQUIRE(!index.isNull());

        WHEN("The sourcefile is parsed with clang include dirs and C++11")
        {
            TranslationUnit tu = index.parse(cpp::test::AstTest::testFile("helloworld.cpp"), CompileOptions()
                .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
                .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
                .std("c++11")
            );

            REQUIRE(!tu.isNull());

            THEN("No diagnostic is generated")
            {
                REQUIRE(tu.numberOfDiagnostics() == 0);
            }
        }
    }
}

SCENARIO("Diagnostics can be displayed with default format settings")
{
    GIVEN("An index and a C++11 sourcefile to parse")
    {
        Index index;
        REQUIRE(!index.isNull());

        WHEN("The sourcefile is parsed with clang include dirs but not C++11 option")
        {
            TranslationUnit tu = index.parse(cpp::test::AstTest::testFile("helloworld-std11.cpp"), CompileOptions()
                .I(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
                .I(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
                .std("c++98")
            );

            REQUIRE(!tu.isNull());

            const auto diagnostics = tu.diagnostics() | ranges::view::bounded;

            THEN("Two diagnostics are generated")
            {
                REQUIRE(tu.numberOfDiagnostics() == 2);

                AND_THEN("The first diagnostic is a warning")
                {
                    CAPTURE(diagnostics[0]);
                    REQUIRE(diagnostics[0].severity() == Diagnostic::Severity::Warning);
                    REQUIRE(diagnostics[0].severityString().str() == "Warning");

                    AND_THEN("The second diagnostic is an error")
                    {
                        CAPTURE(diagnostics[1]);
                        REQUIRE(diagnostics[1].severity() == Diagnostic::Severity::Error);
                        REQUIRE(diagnostics[1].severityString().str() == "Error");
                    }
                }
            }
        }
    }
}
