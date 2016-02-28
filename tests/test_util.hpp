#ifndef CEPLUSPLUS_TESTS_TEST_UTIL_HPP
#define CEPLUSPLUS_TESTS_TEST_UTIL_HPP

#include <gtest/gtest.h>
#include <siplasplas/variant/variant.hpp>

#include <unordered_map>
#include <iostream>

template<typename... Ts>
class TypeList {};

namespace detail
{
    template<typename Src, template<typename...> class T>
    struct functor;

    template<template<typename...> class Src, typename... Ts,
             template<typename...> class T>
    struct functor<Src<Ts...>, T>
    {
        using type = T<Ts...>;
    };

    template<typename Src, template<typename...> class T>
    using functor_t = typename functor<Src, T>::type;

    template<template<typename> class F, typename List>
    struct fmap;

    template<template<typename> class F, typename... Ts>
    struct fmap<F, TypeList<Ts...>>
    {
        using type = TypeList<typename F<Ts>::type...>;
    };

    template<template<typename, typename> class F, typename StartValue, typename List>
    struct fold;

    template<template<typename, typename> class F, typename StartValue, typename Head, typename... Tail>
    struct fold<F, StartValue, TypeList<Head, Tail...>>
    {
       using value = typename F<Head, StartValue>::type;

       using type = typename fold<F, value, TypeList<Tail...>>::type;
    };

    template<template<typename, typename> class F, typename Value>
    struct fold<F, Value, TypeList<>>
    {
        using type = Value;
    };

    template<template<typename> class F, typename List>
    using fmap_t = typename fmap<F, List>::type;

    template<template<typename, typename> class F, typename StartValue, typename List>
    using fold_t = typename fold<F, StartValue, List>::type;


    template<typename Lhs, typename Rhs>
    struct joiner;

    template<template<typename...> class Lhs, template<typename...> class Rhs,
             typename... LhsTs, typename... RhsTs>
    struct joiner<Lhs<LhsTs...>, Rhs<RhsTs...>>
    {
        using type = Lhs<LhsTs..., RhsTs...>;
    };

    template<typename List>
    using join = fold_t<joiner, TypeList<>, List>;

    template<typename... Ts>
    using join_pack = join<TypeList<Ts...>>;
    
    // Functor doesn't work with fixed-size gtest typelists    
    template<template<typename...> class Src, typename... Ts>
    struct functor<Src<Ts...>, ::testing::Types>
    {
        using type = ::testing::Types<Ts...>;
    };
}

template<typename Variant, typename T>
struct VariantTestTraits
{
    using variant = Variant;
    using value_type = T;
};

template<typename Variant, typename T>
struct DefaultVariantTestCase : public VariantTestTraits<Variant, T>
{
    static T getValue()
    {
       return T{};
    }
};

template<typename Variant, typename T>
struct VariantTestCase : public DefaultVariantTestCase<Variant, T>
{};

namespace test_case
{
    template<typename Case>
    using Variant = typename Case::variant;

    template<typename Case>
    using value_type = typename Case::value_type;
}

template<typename Variant>
struct GenerateTestCasesForVariant
{
    using types = detail::functor_t<Variant, TypeList>;

    template<typename T>
    struct MakeUseCase
    {
        using type = VariantTestCase<Variant, T>;
    };

    using type = detail::fmap_t<MakeUseCase, types>;
};

template<typename Variants>
using TestCases = detail::join<detail::fmap_t<GenerateTestCasesForVariant,Variants>>; 


template<typename T, typename = void>
class LifetimeRegistered : public T
{
public:
    template<typename... Args>
    LifetimeRegistered(Args&&... args) :
        T{std::forward<Args>(args)...},
        _id{_latestId++}
    {
        _lifetimeRegistry[_id] = false;
    }

    ~LifetimeRegistered()
    {
        _lifetimeRegistry[_id] = true;
    }

    static bool latestWasDestroyed()
    {
        if(_latestId == 0)
            throw std::runtime_error{"No objects were instanced yet!"};
        else
            return _lifetimeRegistry[_latestId - 1];
    }

private:
    static int _latestId;
    static std::unordered_map<int, bool> _lifetimeRegistry;
    int _id;
};

template<typename T, typename U>
int LifetimeRegistered<T, U>::_latestId = 0;

template<typename T, typename U>
std::unordered_map<int, bool> LifetimeRegistered<T, U>::_lifetimeRegistry;

template<typename T>
struct MakeClass
{
    template<typename... Args>
    MakeClass(Args&&... args) :
        _value{std::forward<Args>(args)...}
    {}

    operator const T&() const
    {
        return _value;
    }

    operator T&()
    {
        return _value;
    }
private:
    T _value;
};

template<typename T>
class LifetimeRegistered<T, std::enable_if_t<!std::is_class<T>::value>> :
    public LifetimeRegistered<MakeClass<T>>
{
public:
    template<typename... Args>
    LifetimeRegistered(Args&&... args) :
        LifetimeRegistered<MakeClass<T>>(MakeClass<T>{std::forward<Args>(args)...})
	{}
};


template<typename Variant>
struct make_test_variant;

template<typename... Ts>
struct make_test_variant<cpp::Variant<Ts...>>
{
    using type = cpp::Variant<LifetimeRegistered<Ts>...>;
};

template<typename Variant>
using make_test_variant_t = typename make_test_variant<Variant>::type;


template<typename T>
LifetimeRegistered<std::decay_t<T>> lifetime_registered(T&& value)
{
    return { std::forward<T>(value) };
}


#endif // CEPLUSPLUS_TESTS_TEST_UTIL_HPP
