#include "asttest.hpp"
#include <fstream>
#include <siplasplas/utility/exception.hpp>

using namespace ::cpp::test;
using namespace ::cpp::constexp;

AstTest::AstTest(const ConstStringView& helloWorldFileName)
{
    helloWorld(helloWorldFileName);
}

void AstTest::writeFile(const ConstStringView& filePath, const ConstStringViews& contents)
{
    std::ofstream os{filePath.c_str()};

    if(os)
    {
        for(const char* line : contents)
        {
            os << line << std::endl;
        }
    }
    else
    {
        throw cpp::exception<std::runtime_error>("cannot open file '{}'", filePath);
    }
}

void AstTest::helloWorld(const cpp::constexp::ConstStringView& fileName)
{
    writeFile(fileName, {
        "#include <iostream>",
        "",
        "int main()",
        "{",
        "    for(const char* word : {\"hello\", \"world\"})",
        "        std::cout << word;",
        "}"
    });
}

std::string AstTest::testFile(const std::string& filename)
{
    return SIPLASPLAS_REFLECTION_PARSER_API_CORE_TESTUTILS_SOURCEDIR + ("/" + filename);
}
