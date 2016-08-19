---
layout: standardese-doc
---

# Header file `fusion.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_FUSION_HPP 

#include "tuple.hpp"

#include "meta.hpp"

#include <bitset>

#include <sstream>

#include <vector>

#include <ctti/type_id.hpp>

namespace cpp
{
    namespace detail
    {
        template <typename T>
        class DefaultConstructible;
        
        template <typename Function, typename Arg, typename ... Args>
        void foreach(Function function, Arg&& arg, Args&&... args);
        
        template <typename Function>
        void foreach(Function);
    }
    
    template <typename Arg>
    auto foreach(Arg&& arg);
    
    template <typename Arg, typename Arg2, typename ... Args>
    auto foreach(Arg&& arg, Arg2&& arg2, Args&&... args);
    
    inline foreach();
    
    namespace 
    {
        template <typename T, typename ... Ts>
        class TypesCall<cpp::meta::list<T, Ts...>;
        
        template <>
        class TypesCall<cpp::meta::list<>;
        
        template <typename ... Ts>
        class TypesCall<cpp::meta::list<cpp::meta::list<Ts...>>>;
    }
    
    template <typename T, typename ... Ts, typename Function>
    std::vector<T> types_call();
    
    template <typename ... Ts, typename Function>
    void foreach_type(Function function);
    
    template <typename Function, typename Previous>
    auto fold(Function function, Previous&& previous);
    
    template <typename Function, typename Previous, typename Head, typename ... Tail>
    auto fold(Function function, Previous&& previous, Head&& head, Tail&&... tail);
    
    template <typename T, typename Function, typename ... Args>
    std::vector<T> fmap();
    
    template <typename Types>
    std::string printTypeList();
}
```
