#include "translationunit.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;

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
