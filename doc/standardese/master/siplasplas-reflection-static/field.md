---
layout: standardese-doc
---

# Header file `field.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP 

#include <siplasplas/utility/meta.hpp>

namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename Class, typename T, T Class::* field, std::size_t Offset>
            class Field<SourceInfo_, T Class::*, field, Offset>;
        }
        
        namespace codegen
        {
            template <typename FieldType, FieldType field>
            class Field;
        }
        
        template <typename FieldType, FieldType field>
        class Field;
    }
}
```
