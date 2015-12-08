//
// Created by manu343726 on 8/12/15.
//

#ifndef CEPLUSPLUS_VARIANT_VISITOR_HPP
#define CEPLUSPLUS_VARIANT_VISITOR_HPP

namespace cpp
{
    template<typename Result>
    struct VariantVisitor
    {
        using ResultType = Result;
    };

    template<typename Result, typename F, typename... Fs>
    struct GenericVariantVisitor : public F, public GenericVariantVisitor<Result, Fs...>
    {
        GenericVariantVisitor(F f, Fs... fs) :
            F{f},
            GenericVariantVisitor<Result, Fs...>{fs...}
        {}

        using F::operator();
        using GenericVariantVisitor<Result, Fs...>::operator();
    };

    template<typename Result, typename F>
    struct GenericVariantVisitor<Result, F> : public VariantVisitor<Result>, public F
    {
        GenericVariantVisitor(F f) :
            F{f}
        {}

        using F::operator();
    };

    template<typename Result, typename... Fs>
    GenericVariantVisitor<Result, Fs...> visitor(Fs... fs)
    {
        return { fs... };
    };
}

#endif //CEPLUSPLUS_VARIANT_VISITOR_HPP
