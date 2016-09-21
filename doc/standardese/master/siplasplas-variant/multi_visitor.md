---
layout: standardese-doc
---

# Header file `multi_visitor.hpp`

``` cpp
#include "visitor.hpp"

namespace cpp
{
    namespace detail
    {
        template <typename Head, typename ... Tail, std::size_t ... Is>
        auto tuple_tail(const std::tuple<Head, Tail...>& tuple, std::index_sequence<Is...>);
        
        template <typename Head, typename ... Tail>
        auto tuple_tail(const std::tuple<Head, Tail...>& tuple);
        
        template <typename Visitor, typename MatchedValues, typename Variants>
        MultiVisitorProcessor<Visitor, MatchedValues, Variants> make_visitor_processor(Visitor&& visitor, MatchedValues&& matchedValues, Variants&& variants);
        
        template <typename Visitor, typename MatchedValues, typename Variant, typename ... Tail>
        class MultiVisitorProcessor<Visitor, MatchedValues, std::tuple<Variant, Tail...>>
        {
        public:
            MultiVisitorProcessor(Visitor&& visitor, MatchedValues&& matchedValues, std::tuple<Variant, Tail...>&& variants);
            
            using ResultType = typename std::decay_t<Visitor>::ResultType;
            
            template <typename T>
            auto next_visitor(T&& value);
            
            template <typename T>
            ResultType operator()(T&& value);
        };
        
        template <typename Visitor, typename MatchedValues>
        class MultiVisitorProcessor<Visitor, MatchedValues, std::tuple<>>
        {
        public:
            MultiVisitorProcessor(Visitor&& visitor, MatchedValues&& matchedValues, std::tuple<>&& variant);
            
            using ResultType = typename std::decay_t<Visitor>::ResultType;
            
            template <typename T>
            ResultType operator()(T&& value);
            
            template <typename T, std::size_t ... Is>
            ResultType call_multivisitor(T&& value, std::index_sequence<Is...>);
        };
    }
    
    template <typename T, typename ... Visitors>
    auto multi_visitor(Visitors&&... visitors);
}
```
