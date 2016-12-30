#ifndef SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_CLANGTEST_HPP
#define SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_CLANGTEST_HPP

#include <siplasplas/reflection/parser/api/core/clang/index.hpp>

namespace cpp
{

namespace test
{

class ClangTest
{
public:
    ClangTest() = default;
    ClangTest(const cpp::constexp::ConstStringView& helloWorldFileName);

    ::cpp::reflection::parser::api::core::clang::Index index;

    static void writeFile(const cpp::constexp::ConstStringView& fileName, const cpp::constexp::ConstStringViews& lines);
    static void helloWorld(const cpp::constexp::ConstStringView& fileName);
};

}

}

#endif // SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_CLANGTEST_HPP
