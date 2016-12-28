#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_COMPILEOPTIONS_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_COMPILEOPTIONS_HPP

#include <siplasplas/reflection/parser/api/core/clang/export.hpp>
#include <siplasplas/constexpr/stringview.hpp>
#include <siplasplas/utility/staticif.hpp>
#include "logger.hpp"

#include <string>
#include <vector>
#include <unordered_set>

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
 * \brief Represents a set of clang command line compile options
 *
 * This class provides a simple interface to feed libclang with a
 * compile options array. A CompileOptions objects have different
 * method to add options, set the C++ standard, include directories,
 * etc. The class also keep tracks that compile options are unique and
 * are not set twice.
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT class CompileOptions
{
public:
    CompileOptions() = default;
    CompileOptions(const cpp::constexp::ConstStringViews& compileOptions);

    /**
     * \brief Adds a compile option
     *
     * \param option String with the full compile option. Symbols such as
     * option prefixes '-' etc are not added by the function but must be included
     * in the option string itself.
     */
    CompileOptions& option(std::string option);

    /**
     * \brief Adds a compile option. Equivalent to option().
     */
    CompileOptions& operator()(std::string option);

    /**
     * \brief Adds an include directory option. Equivalent to
     * option("-I" + includeDir)
     *
     * \param includeDir Full path to the include directory
     */
    CompileOptions& includeDir(const cpp::constexp::ConstStringView& includeDir);

    /**
     * \brief Adds an include directory option. Equivalent to includeDir()
     *
     * \param includeDir Full path to the include directory
     */
    CompileOptions& I(const cpp::constexp::ConstStringView& includeDir);

    /**
     * \brief Adds a compile definition option with the given name and value
     *
     * This functions adds a compile definition (-D) option to the set of clang
     * command line options. If the value is an string, values are passed quoted as
     * in '-DFOO_STR="foo"', else values are passed without quotes '-DFOO=42'
     *
     * \param name Name of the macro that will be defined
     * \param value Value of the macro
     */
    template<typename T>
    CompileOptions& define(const cpp::constexp::ConstStringView& name, const T& value)
    {
        cpp::staticIf<std::is_constructible<cpp::constexp::ConstStringView, T>::value>([&, this](auto identity)
        {
            this->option(fmt::format("-D{}=\"{}\"", name, value));
        }).Else([&, this](auto identity)
        {
            this->option(fmt::format("-D{}={}", name, value));
        });

        return *this;
    }

    /**
     * \brief Adds a compile definition option with the given name and value.
     * Equivalent to define()
     *
     * \param name Name of the macro that will be defined
     * \param value Value of the macro
     */
    template<typename T>
    CompileOptions& D(const cpp::constexp::ConstStringView& name, const T& value)
    {
        return define(name, value);
    }

    /**
     * \brief Adds a c++ standard version compile option. Equivalent to
     * option("-std=" + standard)
     *
     * \param standard Standard version ("c++11", "c++14", etc)
     */
    CompileOptions& std(const cpp::constexp::ConstStringView& standard);

    /**
     * \brief Returns the set of compile options
     *
     * \returns The set of compile options as a C array of null-terminated C strings
     */
    const char* const* options() const;

    /**
     * \brief Returns the number of compile options defined
     */
    std::size_t count() const;

    using OptionPositionPair = std::pair<std::string, std::size_t>;

    struct OptionPositionPairHash
    {
        std::size_t operator()(const OptionPositionPair& pair) const;
    };

    friend std::ostream& operator<<(std::ostream& os, const CompileOptions& compileOptions);

private:
    std::unordered_set<OptionPositionPair, OptionPositionPairHash> _options;
    std::vector<const char*> _rawOptions;
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_COMPILEOPTIONS_HPP
