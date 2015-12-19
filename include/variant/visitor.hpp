#ifndef SIPLASPLAS_VARIANT_VISITOR_HPP
#define SIPLASPLAS_VARIANT_VISITOR_HPP

#include "utility/function.hpp"

namespace cpp
{
    namespace { template<typename...> using void_t = void; }

    template<typename R>
    struct VariantVisitorTraits
    {
        using ResultType = R;
    };

    template<typename R, typename F, typename = void>
    struct VariantVisitor : public F, public VariantVisitorTraits<R>
    {
        using F::F;
        using F::operator();
    };

    template<typename R, typename F>
    struct VariantVisitor<R, F, void_t<typename F::ResultType>> : public F
    {
        using F::F;
        using F::operator();
    };

    template<typename R, typename... Fs>
    VariantVisitor<R, Function<std::decay_t<Fs>...>> visitor(Fs&&... fs)
    {
        return { std::forward<Fs>(fs)... };
    }
}

#endif // SIPLASPLAS_VARIANT_VISITOR_HPP
