#include "translationunit.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

String TranslationUnit::spelling() const
{
    return {::clang_getTranslationUnitSpelling(rawHandle())};
}

std::size_t TranslationUnit::numberOfDiagnostics() const
{
    return ::clang_getNumDiagnostics(rawHandle());
}

bool TranslationUnit::noDiagnostics() const
{
    return numberOfDiagnostics() == 0;
}

Cursor TranslationUnit::cursor() const
{
    return ::clang_getTranslationUnitCursor(rawHandle());
}

TranslationUnit::Inclusion::Inclusion(const File& file, std::vector<SourceLocation>&& stack) :
    _includedFile{file},
    _includeStack{std::move(stack)}
{}

const File& TranslationUnit::Inclusion::file() const
{
    return _includedFile;
}

ConstArrayView<SourceLocation> TranslationUnit::Inclusion::stack() const
{
    return { &_includeStack[0], &_includeStack[_includeStack.size()] };
}

std::vector<TranslationUnit::Inclusion> TranslationUnit::inclusions() const
{
    std::vector<TranslationUnit::Inclusion> inclusions;

    ::clang_getInclusions(rawHandle(), [](::CXFile includedFile, ::CXSourceLocation* inclusionStack, unsigned inclusionStackLength, void* userData)
    {
        auto* inclusions = reinterpret_cast<std::vector<TranslationUnit::Inclusion>*>(userData);
        std::vector<SourceLocation> stack;
        stack.reserve(inclusionStackLength);

        for(std::size_t i = 0; i < inclusionStackLength; ++i)
        {
            stack.emplace_back(inclusionStack[i]);
        }

        inclusions->emplace_back(includedFile, std::move(stack));
    }, &inclusions);

    return inclusions;
}
