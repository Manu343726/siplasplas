---
layout: standardese-doc
---

# Header file `sourceinfo.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP 

#include <siplasplas/utility/meta.hpp>

#include <array>

#include <string>

namespace cpp
{
    namespace static_reflection
    {
        enum class Kind;
        
        namespace meta
        {
            template <static_reflection::Kind Kind, typename FullName, typename Spelling, typename DisplayName, typename File, std::size_t Line>
            class SourceInfo;
        }
    }
}
```

## Enumeration `cpp::static_reflection::Kind`<a id="cpp::static_reflection::Kind"></a>

``` cpp
enum class Kind
{
    NAMESPACE,
    FUNCTION,
    ENUM,
    CLASS,
    FIELD,
    UNKNOWN,
};
```

See cpp::static\_reflection::meta::SourceInfo.

-----

## Class template `cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>`<a id="cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>"></a>

``` cpp
template <static_reflection::Kind Kind, typename FullName, typename Spelling, typename DisplayName, typename File, std::size_t Line>
class SourceInfo
{
public:
    static constexpr static_reflection::Kind kind();
    
    static constexpr const char* fullName();
    
    static constexpr const char* spelling();
    
    static constexpr const char* displayName();
    
    static constexpr const char* file();
    
    static constexpr std::size_t line();
};
```

SourceInfo class stores information about the declaration of an entity (class, function, etc) in the context of the sourcecode where the entity is declared. This information includes the file where the entity is declared, the line, the name of the entity, etc.

### Function `cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::kind`<a id="cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::kind"></a>

``` cpp
static constexpr static_reflection::Kind kind();
```

Retrns the kind (category) of the entity

``` cpp
auto kind = cpp::static_reflection::Class<MyClass>::SourceInfo::kind(); // Returns Kind::CLASS
```

-----

### Function `cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::fullName`<a id="cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::fullName"></a>

``` cpp
static constexpr const char* fullName();
```

Returns the full qualified name of an entity \\returns a pointer to a null-terminated `constexpr` C string with the full name. If the source info is a default empty source info (Such as from an entity without reflection metadata available) returns an empty string.

-----

### Function `cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::spelling`<a id="cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::spelling"></a>

``` cpp
static constexpr const char* spelling();
```

Returns the name of the entity See [`clang_getCursorSpelling()`](http://clang.llvm.org/doxygen/group__CINDEX__CURSOR__XREF.html#gaad1c9b2a1c5ef96cebdbc62f1671c763).

*Returns:* a pointer to a null-terminated `constexpr` C string with the spelling. If the source info is a default empty source info (Such as from an entity without reflection metadata available) returns an empty string.

-----

### Function `cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::displayName`<a id="cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::displayName"></a>

``` cpp
static constexpr const char* displayName();
```

Returns the display name of the entity.

The display name contains extra information that helps identify the entity, such as the parameters of a function or template or the arguments of a class template specialization.

See [`clang_getCursorDisplayName()`](http://clang.llvm.org/doxygen/group__CINDEX__CURSOR__XREF.html#gac3eba3224d109a956f9ef96fd4fe5c83)

*Returns:* a pointer to a null-terminated `constexpr` C string with the display name. If the source info is a default empty source info (Such as from an entity without reflection metadata available) returns an empty string.

-----

### Function `cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::file`<a id="cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::file"></a>

``` cpp
static constexpr const char* file();
```

Returns the full path to the file where the entity is declared.

The path returned is the full path to the declaration file of the entity **as read when the file was processed by DRLParser**. This full path may change if the user sources are deployed to a different path/machine, so user code should not rely in this full path. However, depending on the structure of the user project, a relative path to the user source tree or include tree may be get from this path

*Returns:* a pointer to a null-terminated `constexpr` C string with the full path. If the source info is a default empty source info (Such as from an entity without reflection metadata available) returns an empty string.

-----

### Function `cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::line`<a id="cpp::static_reflection::meta::SourceInfo<Kind, FullName, Spelling, DisplayName, File, Line>::line"></a>

``` cpp
static constexpr std::size_t line();
```

Returns the line number where the entity is declared

-----

-----
