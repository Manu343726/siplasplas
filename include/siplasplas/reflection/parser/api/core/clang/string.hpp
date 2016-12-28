#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_STRING_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_STRING_HPP

#include <siplasplas/constexpr/stringview.hpp>
#include <clang-c/CXString.h>
#include "handle.hpp"


bool operator==(const ::CXString& lhs, const ::CXString rhs);
bool operator!=(const ::CXString& lhs, const ::CXString rhs);


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

template<>
class NullHandle<::CXString>
{
public:
    static const ::CXString& value;
};

/**
 * \ingroup clang
 * \brief Unique handle to a libclang CXString pointer
 */
using CXString = core::clang::UniqueHandle<
    ::CXString,
    ::clang_disposeString
>;

/**
 * \ingroup clang
 * \brief Interface to a string managed by the libclang C interface
 */
class String : public core::clang::CXString
{
public:
    using core::clang::CXString::CXString;

    /**
     * \brief Returns a string view to the string data
     */
    cpp::constexp::ConstStringView str() const;

    /**
     * \brief Returns a C string from the libclang string
     */
    const char* c_str() const;

    friend std::ostream& operator<<(std::ostream& os, const String& str);
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_STRING_HPP
