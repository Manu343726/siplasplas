#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_ERROR_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_ERROR_HPP

#include <siplasplas/constexpr/stringview.hpp>
#include <clang-c/CXErrorCode.h>
#include <siplasplas/reflection/parser/api/core/clang/export.hpp>

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

/**
 * \ingroup clang
 * \brief Represents a libclang API error
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT
class Error
{
public:
    /**
     * \brief Error code enumeration
     *
     * The int error codes are the same of CXError from libclang
     */
    enum class Code
    {
        Success = ::CXError_Success,
        Failure = ::CXError_Failure,
        Crashed = ::CXError_Crashed,
        InvalidArguments = ::CXError_InvalidArguments,
        ASTReadError = ::CXError_ASTReadError
    };

    friend std::ostream& operator<<(std::ostream& os, const Error::Code errorCode);
    friend std::ostream& operator<<(std::ostream& os, const Error& error);

    /**
     * \brief Initializes an Error given a libclang CXError error code
     */
    Error(::CXErrorCode cxErrorCode);

    /**
     * \brief Returns an string diagnostic or the error
     */
    cpp::constexp::ConstStringView diagnostic() const;

    /**
     * \brief Returns an string with error details
     */
    cpp::constexp::ConstStringView details() const;

    /**
     * \brief Returns the equivalent libclang CXErrror enum error code
     */
    ::CXErrorCode cxErrorCode() const;

    /**
     * \brief Returns the error code
     */
    Code errorCode() const;

    /**
     * \brief Returns whether the operation associated to the error code was
     * completed succesfully
     *
     * \returns True if there was no error, false if there was an error.
     */
    bool success() const;

private:
    ::CXErrorCode _cxErrorCode;
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_ERROR_HPP
