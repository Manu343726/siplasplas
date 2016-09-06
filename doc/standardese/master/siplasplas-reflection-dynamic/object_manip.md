---
layout: standardese-doc
---

# Header file `object_manip.hpp`

``` cpp
#include "object.hpp"

#include "logger.hpp"

#include <siplasplas/utility/meta.hpp>

#include <siplasplas/utility/function_traits.hpp>

#include <siplasplas/utility/assert.hpp>

#include <vector>

namespace cpp
{
    namespace dynamic_reflection
    {
        namespace detail
        {
            template <typename R, typename Class, typename ... Args>
            class ConstInvokeVectorCallOnMember;
            
            template <typename R, typename Class, typename ... Args>
            class InvokeVectorCallOnMember;
            
            template <typename ... Args, std::size_t ... Is>
            std::vector<cpp::dynamic_reflection::Object> tuple_to_vector();
        }
        
        template <typename ... Args>
        std::vector<cpp::dynamic_reflection::Object> pack_to_vector();
        
        template <typename ... Args>
        std::tuple<Args...> vector_to_tuple(const std::vector<cpp::dynamic_reflection::Object>& vector);
        
        template <typename ... Args>
        std::vector<cpp::dynamic_reflection::Object> tuple_to_vector();
        
        template <typename Function>
        auto vector_call(Function function, const std::vector<cpp::dynamic_reflection::Object>& vector);
        
        namespace detail{}
    }
}
```
