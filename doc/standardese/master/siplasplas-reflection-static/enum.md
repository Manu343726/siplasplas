---
layout: standardese-doc
---

# Header file `enum.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP 

#include <siplasplas/utility/meta.hpp>

#include <string>

#include <limits>

#include <array>

namespace cpp
{
    namespace static_reflection
    {
        namespace meta{}
        
        namespace codegen{}
        
        template <typename EnumType>
        class Enum;
    }
}
```

## Class template `cpp::static_reflection::Enum<EnumType>`<a id="cpp::static_reflection::Enum<EnumType>"></a>

``` cpp
template <typename EnumType>
class Enum
: public codegen::Enum<EnumType>
{};
```

See cpp::static\_reflection::meta::Enum template for the returned information.

### Template parameter `cpp::static_reflection::Enum<EnumType>::EnumType`<a id="cpp::static_reflection::Enum<EnumType>::EnumType"></a>

``` cpp
typename EnumType
```

Must be an enumeration type. The behavior is undefined otherwise.

-----
