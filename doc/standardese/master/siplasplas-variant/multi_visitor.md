
---
layout: standardese-doc
---

# Header file `multi_visitor.hpp`

``` cpp
#define SIPLASPLAS_VARIANT_MULTI_VISITOR_HPP 

#include <tuple>

#include "visitor.hpp"

namespace cpp
{
    namespace detail
    {
        template <typename Head, typename Tail, std::size_t ... Is>
        auto tuple_tail(const std::<Head, Tail...>& tuple, std::index_sequence<Is...>);
        
        template <typename Head, typename Tail>
        auto tuple_tail(const std::<Head, Tail...>& tuple);
        
        template <typename Visitor, typename MatchedValues, typename Variants>
        MultiVisitorProcessor<Visitor,
        MatchedValues,
        Variants> make_visitor_processor(Visitor&& visitor, MatchedValues&& matchedValues, Variants&& variants);
        
        template <typename Visitor, typename MatchedValues, typename Variant, typename Tail>
        class MultiVisitorProcessor<Visitor, MatchedValues, std::tuple<Variant, Tail...>;
        
        template <typename Visitor, typename MatchedValues>
        class MultiVisitorProcessor<Visitor, MatchedValues, std::tuple<>;
    }
    
    template <typename T, typename Visitors>
    auto multi_visitor(Visitors&&... visitors);
}
```
