#include "diagnostic.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

int Diagnostic::category() const
{
    return ::clang_getDiagnosticCategory(rawHandle());
}

String Diagnostic::categoryText() const
{
    return ::clang_getDiagnosticCategoryText(rawHandle());
}

String Diagnostic::display() const
{
    return ::clang_formatDiagnostic(rawHandle(), ::clang_defaultDiagnosticDisplayOptions());
}

String Diagnostic::display(const ConstArrayView<Diagnostic::DisplayOption>& displayOptions) const
{
    unsigned options = 0;

    for(const DisplayOption& option : displayOptions)
    {
        options |= static_cast<unsigned>(option);
    }

    return ::clang_formatDiagnostic(rawHandle(), options);
}
