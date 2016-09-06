---
layout: standardese-doc
---

# Header file `attribute.hpp`

``` cpp
#include <vector>

#include <siplasplas/reflection/dynamic/object.hpp>

#include <siplasplas/reflection/dynamic/object_manip.hpp>

#include <siplasplas/utility/type_variables.hpp>

#include <siplasplas/reflection/dynamic/export.hpp>

namespace cpp
{
    namespace dynamic_reflection
    {
        namespace attributes
        {
            class Attribute;
            
            template <typename AttrClass>
            class ImplementAttribute;
            
            template <typename R, typename Class, typename ... Args>
            class AttributeFor<R(Class::*)(Args...)>;
            
            template <template <typename> typename Attribute, typename ... Args>
            auto attribute_template_builder(Args&&... args);
        }
    }
}
```
