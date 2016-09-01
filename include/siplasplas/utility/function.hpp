//
// Created by manu343726 on 8/12/15.
//

#ifndef SIPLASPLAS_UTILITY_FUNCTION_HPP
#define SIPLASPLAS_UTILITY_FUNCTION_HPP

#include "meta.hpp"
#include <utility>

namespace cpp
{
    template<typename... Fs>
    struct Function;

    template<typename F>
    struct Function<F> : public F
    {
        using F::operator();

        template<typename _F, typename... _Fs>
        Function(_F&& f, _Fs&&... fs) :
            F{std::forward<_F>(f)} 
        {}
    };

    template<typename R, typename... Args>
    struct Function<R(Args...)>
    {
        template<typename _F>
        Function(_F&& f) :
            _f{std::forward<_F>(f)}
        {}

        template<typename... _Args>
        R operator()(_Args&&... args) const
        {
            return _f(std::forward<_Args>(args)...);
        }

    private:
        R(*_f)(Args...);
    };

    template<typename First, typename Second, typename... Tail>
    struct Function<First, Second, Tail...> : public First, public Function<Second, Tail...>
    {
        using First::operator();
        using Function<Second, Tail...>::operator();

        template<typename _First, typename _Second, typename... _Tail>
        Function(_First&& first, _Second&& second, _Tail&&... tail) :
            First(std::forward<_First>(first)),
            Function<Second, Tail...>( std::forward<_Second>(second), std::forward<Tail>(tail)...)
        {}
    };

    template<typename... Fs>
    Function<cpp::meta::decay_t<Fs>...> make_function(Fs&&... fs)
    {
        return { std::forward<Fs>(fs)... };
    }
}

#endif //SIPLASPLAS_UTILITY_FUNCTION_HPP
