---
layout: standardese-doc
---

# Header file `class.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP 

#include "sourceinfo.hpp"

#include <siplasplas/utility/meta.hpp>

namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename Class_, typename Methods_, typename Fields_, typename Constructors_, typename Classes_, typename Enums_>
            class Class;
        }
        
        namespace codegen
        {
            template <typename T>
            class Class;
        }
        
        template <typename T>
        class Class;
    }
}
```

## Class template `cpp::static_reflection::Class<T>`<a id="cpp::static_reflection::Class<T>"></a>

``` cpp
template <typename T>
class Class
: public codegen::Class<T>
{
};
```

This template returns (inherits) a cpp::static\_reflection::meta::Class instance with the information of the given class type. If there's no static reflection information of this class in the current translation unit, returns an empty cpp::static\_reflection::meta::Class instance (Default "unknown" source info, empty methods, ctors, enums, and classes lists, etc). The behavior is undefined if the given type is not a class type.

See cpp::static\_reflection::meta::Class for the returned information.

``` cpp

template<typename T, typename = std::enable_if_t<!std::is_basic_type<T>::value>>
std::string serialize(const T& object)
{
    std::ostringstream os;

    os << cpp::static_reflection::Class<T>::fullName() << ": {";

    cpp::foreach_type<typename cpp::static_reflection::Class<T>::Fields>([&](auto type)
    {
        using FieldInfo = cpp::meta::type_t<decltype(type)>;

        os << serialize(object.*FieldInfo::get()) << ", ";
    });

    os << "}";

    return os.str();
}

template<typename T, std::enable_if_t<std::is_basic_type<T>::value>>
std::string serialize(T value)
{
    std::ostream os;
    os << ctti::type_id<T>().name() << ": " << value;
    return os.str();
}

template<typename T>
std::string serialize(const std::vector<T>& values)
{
    std::ostream os;

    os << "[";

    for(const T& value : values)
    {
        os << serialize(value) << ", ";
    }

    os << "]";

    return os.str();
}

std::string serialize(const std::string& value)
{
    return value;
}

```

### Template parameter `cpp::static_reflection::Class<T>::T`<a id="cpp::static_reflection::Class<T>::T"></a>

``` cpp
typename T
```

Must be a class type.

-----
