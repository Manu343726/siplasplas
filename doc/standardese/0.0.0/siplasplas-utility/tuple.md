---
layout: standardese-doc
---

# Header file `tuple.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_TUPLE_HPP 

#include <tuple>

#include "meta.hpp"

namespace cpp
{
    namespace 
    {
        template <typename Function, typename ... Args, std::size_t ... Is>
        constexpr auto tuple_call(Function function, const std::tuple<Args...>& tuple, meta::index_sequence<Is...>);
        
        template <typename Head, typename ... Tail, std::size_t ... Is>
        constexpr auto tuple_tail(const std::tuple<Head, Tail...>& tuple, std::index_sequence<Is...>);
    }
    
    template <typename Function, typename ... Args>
    constexpr auto tuple_call(Function function, const std::tuple<Args...>& tuple);
    
    template <typename Function, typename ... Args>
    constexpr auto tuple_call(const std::tuple<Args...>& tuple, Function function);
    
    template <typename Head, typename ... Tail>
    constexpr auto tuple_tail(const std::tuple<Head, Tail...>& tuple);
}
```
