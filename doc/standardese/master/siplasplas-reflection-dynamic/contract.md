---
layout: standardese-doc
---

# Header file `contract.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_ATTRIBUTES_CONTRACT_HPP 

#include "attribute.hpp"

namespace cpp
{
    namespace dynamic_reflection
    {
        namespace attributes
        {
            template <typename ReturnType, typename ... Args>
            class Contract<ReturnType, meta::list<Args...>;
            
            template <typename Precond, typename Postcond>
            std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute> contract(Precond precond, const std::string& precondMsg, Postcond postcond, const std::string& postcondMsg);
        }
    }
}
```
