---
layout: standardese-doc
---

# Header file `enum.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP 

#include <siplasplas/utility/meta.hpp>

#include <string>

namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename EnumType, EnumType ... Constants, typename ... ConstantsNames>
            class Enum<
            SourceInfo_,
            EnumType,
            ::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,
            ::cpp::meta::list<ConstantsNames...>
            >;
        }
        
        namespace codegen{}
        
        template <typename EnumType>
        class Enum;
    }
}
```
