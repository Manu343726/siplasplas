#ifndef SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP
#define SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP

#include <siplasplas/constexpr/string.hpp>
#include <siplasplas/constexpr/meta.hpp>
#include <siplasplas/utility/meta.hpp>
#include <array>
#include <string>

namespace cpp
{

namespace static_reflection
{

/**
 * \ingroup static-reflection
 * \brief Represents the different types of entities
 * (namespaces, classes, functions, etc) the library
 * can collect static reflection information.
 *
 * See cpp::static_reflection::meta::SourceInfo.
 */
enum class Kind
{
    NAMESPACE,
    FUNCTION,
    ENUM,
    CLASS,
    FIELD,
    UNKNOWN
};

namespace meta
{

/**
 * \ingroup static-reflection
 * \brief Stores source information of an entity
 *
 * SourceInfo class stores information about the declaration of an entity (class, function, etc) in the context
 * of the sourcecode where the entity is declared. This information includes the file where the entity is declared, the line, the name
 * of the entity, etc.
 */
template<
    typename Entity_,
    static_reflection::Kind Kind,
    typename FullName,
    typename Spelling,
    typename DisplayName,
    typename File,
    std::size_t Line
>
class SourceInfo
{
private:

public:
    /**
     * \brief Returns the entity (Or a type representation of the entity)
     */
    using Entity = Entity_;

    /**
     * \brief Returns the kind (category) of the entity
     *
     * ``` cpp
     * auto kind = cpp::static_reflection::Class<MyClass>::SourceInfo::kind(); // Returns Kind::CLASS
     * ```
     */
    static constexpr static_reflection::Kind kind()
    {
        return Kind;
    }

    /**
     * \brief Returns the full qualified name of an entity
     * \returns a constexpr string view with the full name.
     * If the source info is a default empty source info (Such as from an entity without reflection
     * metadata available) returns an empty string.
     */
    static constexpr const cpp::constexp::String<FullName::size + 1>& fullName()
    {
        return cpp::constexp::SequenceToString<FullName>::str();
    }

    /**
     * \brief Returns the name of the entity
     * See [`clang_getCursorSpelling()`](http://clang.llvm.org/doxygen/group__CINDEX__CURSOR__XREF.html#gaad1c9b2a1c5ef96cebdbc62f1671c763).
     *
     * \returns A constexpr string view with the spelling.
     * If the source info is a default empty source info (Such as from an entity without reflection
     * metadata available) returns an empty string.
     */
    static constexpr const cpp::constexp::String<Spelling::size + 1>& spelling()
    {
        return cpp::constexp::SequenceToString<Spelling>::str();
    }

    /**
     * \brief Returns the display name of the entity.
     *
     * The display name contains extra information that helps identify the entity,
     * such as the parameters of a function or template or the arguments of a class
     * template specialization.
     *
     * See [`clang_getCursorDisplayName()`](http://clang.llvm.org/doxygen/group__CINDEX__CURSOR__XREF.html#gac3eba3224d109a956f9ef96fd4fe5c83)
     *
     * \returns a pointer to a null-terminated `constexpr` C string
     * with the display name.
     * If the source info is a default empty source info (Such as from an entity without reflection
     * metadata available) returns an empty string.
     */
    static constexpr const char* displayName()
    {
        return ::cpp::constexp::SequenceToString<DisplayName>::c_str();
    }

    /**
     * \brief Returns the full path to the file where the entity is declared.
     *
     * The path returned is the full path to the declaration file of the entity **as read
     * when the file was processed by DRLParser**. This full path may change if the user sources are deployed
     * to a different path/machine, so user code should not rely in this full path. However, depending on the structure
     * of the user project, a relative path to the user source tree or include tree may be get from this
     * path
     *
     * \returns a pointer to a null-terminated `constexpr` C string
     * with the full path.
     * If the source info is a default empty source info (Such as from an entity without reflection
     * metadata available) returns an empty string.
     */
    static constexpr const char* file()
    {
        return ::cpp::constexp::SequenceToString<File>::c_str();
    }

    /**
     * \brief Returns the line number where the entity is declared
     */
    static constexpr std::size_t line()
    {
        return Line;
    }
};

template<typename T>
using EmptySourceInfo = SourceInfo<
    T,
    (std::is_enum<T>::value ? Kind::ENUM : Kind::CLASS),
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    0
>;

}

}

}

#endif // SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP
