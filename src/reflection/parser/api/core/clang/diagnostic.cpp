#include "diagnostic.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

namespace
{

const char* severityString(const Diagnostic::Severity severity)
{
    static constexpr const char* strings[] = {
        "Ignored",
        "Note",
        "Warning",
        "Error",
        "Fatal"
    };

    return strings[static_cast<std::size_t>(severity)];
}

}

Diagnostic::Severity Diagnostic::severity() const
{
    return static_cast<Diagnostic::Severity>(::clang_getDiagnosticSeverity(rawHandle()));
}

ConstStringView Diagnostic::severityString() const
{
    return ::severityString(severity());
}

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

namespace cpp
{

namespace reflection
{

namespace parser
{

namespace api
{

namespace core
{

namespace clang
{

std::ostream& operator<<(std::ostream& os, const Diagnostic::Severity severity)
{
    return os << severityString(severity);
}

std::ostream& operator<<(std::ostream& os, const Diagnostic& diagnostic)
{
    return os << diagnostic.display();
}

}

}

}

}

}

}
