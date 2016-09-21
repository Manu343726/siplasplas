---
layout: standardese-doc
---

# Header file `visitor.hpp`

``` cpp
namespace cpp
{
    namespace {}
    
    template <typename R>
    struct VariantVisitorTraits
    {
        using ResultType = R;
    };
    
    template <typename R, typename F, typename = void>
    struct VariantVisitor
    : F, VariantVisitorTraits<R>
    {
        using F::F;
        
        using F::operator();
    };
    
    template <typename R, typename F>
    struct VariantVisitor<R, F, void_t<typename F::ResultType>>
    : F
    {
        using F::F;
        
        using F::operator();
    };
    
    template <typename R, typename ... Fs>
    VariantVisitor<R, Function<std::decay_t<Fs>...>> visitor(Fs&&... fs);
}
```
