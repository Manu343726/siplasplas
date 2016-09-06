---
layout: standardese-doc
---

# Header file `contract.hpp`

``` cpp
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
