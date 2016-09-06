---
layout: standardese-doc
---

# Header file `type.hpp`

``` cpp
#include <siplasplas/reflection/common/type_info.hpp>

#include <siplasplas/allocator/freelist_allocator.hpp>

#include <siplasplas/utility/exception.hpp>

#include <siplasplas/utility/lexical_cast.hpp>

#include <siplasplas/utility/meta.hpp>

#include <siplasplas/reflection/dynamic/export.hpp>

#include <unordered_map>

#include <vector>

#include <algorithm>

#define CPP_REFLECTION_CUSTOM_TYPENAME_FOR(type, name) \
    namespace cpp { namespace detail {                  \
    template<>                                          \
    struct CustomTypeName<type> {                       \
        static constexpr ::ctti::unnamed_type_id_t id() \
        {                                               \
            return ::ctti::id_from_name(name);          \
        }                                               \
    }; }}

#define CPP_REFLECTION_FORCE_TYPENAME(type) CPP_REFLECTION_CUSTOM_TYPENAME_FOR(type, #type)

namespace cpp
{
    namespace dynamic_reflection
    {
        namespace detail
        {
            template <typename T>
            struct CustomTypeName;
        }
        
        class Type;
    }
}
```
