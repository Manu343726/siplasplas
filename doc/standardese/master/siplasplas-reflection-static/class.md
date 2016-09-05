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
        namespace meta{}
        
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

### Template parameter `cpp::static_reflection::Class<T>::T`<a id="cpp::static_reflection::Class<T>::T"></a>

``` cpp
typename T
```

Must be a class type.

-----
