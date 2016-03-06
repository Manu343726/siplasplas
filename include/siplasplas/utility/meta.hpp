#ifndef SIPLASPLAS_UTILITY_META_H
#define SIPLASPLAS_UTILITY_META_H

#include <type_traits>
#include <cstdint>

namespace cpp
{

namespace meta
{
    template<typename... Bs>
    struct assert;

    template<typename B, typename... Bs>
    struct assert<B, Bs...> : assert<Bs...>
    {
        static_assert(B::value, "Assertion failed");
    };

    template<>
    struct assert<> {};

    template<typename T>
    using void_t = typename std::conditional<sizeof(T) >= 0, void, T>::type;

    template<typename T>
    struct identity
    {
        using type = T;
    };

    template<typename Metafunction>
    using type_t = typename Metafunction::type;

    template<typename MetafunctionClass, typename... Args>
    using apply_t = type_t<typename MetafunctionClass::template apply<Args...>>;

    template<typename MetafunctionClass, typename Seq>
    struct sequence_apply;

    template<typename MetafunctionClass,
             template<typename...> class Seq, typename... Ts>
    struct sequence_apply<MetafunctionClass, Seq<Ts...>>
    {
        using type = apply_t<MetafunctionClass, Ts...>;
    };

    template<typename MetafunctionClass, typename Seq>
    using sequence_apply_t = type_t<sequence_apply<MetafunctionClass, Seq>>;

    template<std::uint8_t I>
    using uint8_t = std::integral_constant<std::uint8_t, I>;
    template<std::uint16_t I>
    using uint16_t = std::integral_constant<std::uint16_t, I>;
    template<std::uint32_t I>
    using uint32_t = std::integral_constant<std::uint32_t, I>;
    template<std::uint64_t I>
    using uint64_t = std::integral_constant<std::uint64_t, I>;

    template<std::int8_t I>
    using int8_t = std::integral_constant<std::int8_t, I>;
    template<std::int16_t I>
    using int16_t = std::integral_constant<std::int16_t, I>;
    template<std::int32_t I>
    using int32_t = std::integral_constant<std::int32_t, I>;
    template<std::int64_t I>
    using int64_t = std::integral_constant<std::int64_t, I>;

    template<std::size_t I>
    using size_t = std::integral_constant<std::size_t, I>;

    template<bool B>
    using bool_ = std::integral_constant<bool, B>;
    using true_ = bool_<true>;
    using false_ = bool_<false>;

    template<typename T, typename = void>
    struct is_integral : false_ {};
    template<typename T>
    struct is_integral<T, void_t<decltype(T::value)>> :
        true_
    {};

    template<typename T, typename = assert<is_integral<T>>>
    constexpr decltype(T::value) value()
    {
        return T::value;
    }

    template<template<typename...> class Function>
    struct defer
    {
        template<typename... Args>
        struct apply
        {
            template<typename Instance, bool = is_integral<Instance>::value>
            struct result
            {
                using type = Instance;
            };

            template<typename Instance>
            struct result<Instance, false>
            {
                using type = type_t<Instance>;
            };

            using type = type_t<result<Function<Args...>>>;
        };
    };

    struct and_
    {
        template<typename Lhs, typename Rhs>
        struct apply : assert<
            is_integral<Lhs>, 
            is_integral<Rhs>
        >
        {
            using type = bool_<Lhs::value && Rhs::value>;
        };
    };

    template<typename Lhs, typename Rhs>
    using and_t = apply_t<and_, Lhs, Rhs>;

    struct or_
    {
        template<typename Lhs, typename Rhs>
        struct apply : assert<
            is_integral<Lhs>,
            is_integral<Rhs>
        >
        {
            using type = bool_<Lhs::value || Rhs::value>;
        };
    };

    template<typename Lhs, typename Rhs>
    using or_t = apply_t<or_, Lhs, Rhs>;


	template<typename... Ts>
	struct list
	{
		static constexpr std::size_t size = sizeof...(Ts);
	};


    template<typename Seq>
    struct functor;

    template<template<typename...> class Seq, typename... Ts>
    struct functor<Seq<Ts...>>
    {
        template<typename... Us>
        struct apply
        {
            using type = Seq<Us...>;
        };
    };

    template<typename Seq, typename... Ts>
    using apply_functor = apply_t<functor<Seq>, Ts...>;

    template<typename Lhs, typename Rhs>
    struct cat;
    
    template<template<typename...> class Seq,
             typename... Lhs, typename... Rhs>
    struct cat<Seq<Lhs...>, Seq<Rhs...>>
    {
        using type = Seq<Lhs..., Rhs...>;
    };

    template<typename Lhs, typename Rhs>
    using cat_t = type_t<cat<Lhs, Rhs>>;

    template<std::size_t I>
    using index_t = std::integral_constant<std::size_t, I>;

    template<std::size_t... Is>
    using index_sequence = list<index_t<Is>...>;

    namespace detail
    {
        template<std::size_t N>
        struct make_index_sequence
        {
            using type = cat_t<
                type_t<make_index_sequence<(N % 2) ? ((N-1) / 2) : N / 2>>,
                type_t<make_index_sequence<N / 2>>
            >;
        };

        template<>
        struct make_index_sequence<1>
        {
            using type = index_sequence<0>;
        };

        template<>
        struct make_index_sequence<0>
        {
            using type = index_sequence<>;
        };
    }

    template<std::size_t N>
    using make_index_sequence = type_t<detail::make_index_sequence<N>>;
    template<typename... Ts>
    using make_index_sequence_for = make_index_sequence<sizeof...(Ts)>;

    namespace detail
    {
        template<typename Left, std::size_t Index, typename Right>
        struct split;

        template<template<typename...> class Seq, std::size_t Index,
                 typename... Left, typename Head, typename... Tail>
        struct split<Seq<Left...>, Index, Seq<Head, Tail...>>
        {
            using left  = typename split<Seq<Left..., Head>, Index - 1, Seq<Tail...>>::left;
            using head  = Head;
            using right = typename split<Seq<Left..., Head>, Index - 1, Seq<Tail...>>::right;
        };

        template<template<typename...> class Seq,
            typename... Left, typename Head, typename... Tail>
        struct split<Seq<Left...>, 0, Seq<Head, Tail...>>
        {
            using left  = Seq<Left..., Head>;
            using head  = Head;
            using right = Seq<Tail...>;
        };
    }

    template<std::size_t Index, typename... Ts>
    using split_pack = detail::split<list<>, Index, list<Ts...>>;
    template<std::size_t Index, typename... Ts>
    using split_pack_left_t = typename split_pack<Index, Ts...>::left;
    template<std::size_t Index, typename... Ts>
    using pack_get = typename split_pack<Index, Ts...>::head;
    template<std::size_t Index, typename... Ts>
    using split_pack_right_t = typename split_pack<Index, Ts...>::right;

    template<std::size_t Index, typename Seq>
    struct split;
    template<std::size_t Index, template<typename...> class Seq, typename... Ts>
    struct split<Index, Seq<Ts...>>
    {
        using type = detail::split<Seq<>, Index, Seq<Ts...>>;
    };
    template<std::size_t Index, typename Seq>
    using split_left_t = typename split<Index, Seq>::left;
    template<std::size_t Index, typename Seq>
    using get = typename split<Index, Seq>::head;
    template<std::size_t Index, typename Seq>
    using split_right_t = typename split<Index, Seq>::right;

	template<typename Key, typename Value>
	struct pair
	{
		using key = Key;
		using value = Value;
	};

    template<typename Pair>
    using key_t = typename Pair::key;
    template<typename Pair>
    using value_t = typename Pair::value;

    template<typename... Ts>
    struct inherit : Ts... {};

    template<typename... Ts>
    struct inherit<list<Ts...>> : Ts... {};

	template<typename...>
	struct map;

	template<typename... Keys, typename... Values>
	struct map<pair<Keys, Values>...>
	{
		using keys   = list<Keys...>;
		using values = list<Values...>;
        using pairs  = list<pair<Keys, Values>...>;

        template<typename Key>
        using at_key = type_t<decltype(lookup((inherit<pairs>*)nullptr))>;
	private:
		template<typename Key, typename Value>
        identity<Value> lookup(pair<Key, Value>*);
	};

    template<typename Map>
    using keys_t = typename Map::keys;
    template<typename Map>
    using values_t = typename Map::values;
    template<typename Map>
    using pairs_t = typename Map::pairs;
    template<typename Map, typename Key>
    using at_key = typename Map::template at_key<Key>;

    template<typename Function, typename... Ts>
    struct pack_fmap
    {
        using type = list<apply_t<Function, Ts>...>;
    };

    template<typename Function, typename List>
    struct fmap;

    template<typename Function, template<typename...> class Seq, typename... Ts>
    struct fmap<Function, Seq<Ts...>>
    {
        using type = Seq<apply_t<Function, Ts>...>;
    };

    namespace detail
    {
        template<typename Predicate, typename FilteredSeq, typename Seq>
        struct filter;

        template<typename Predicate,
                 template<typename...> class Seq, typename... Filtered, typename Head, typename... Tail>
        struct filter<Predicate, Seq<Filtered...>, Seq<Head, Tail...>>
        {
            template<typename _Head = Head, bool = value<apply_t<Predicate, _Head>>()>
            struct next
            {
                using type = Seq<Filtered..., Head>;
            };

            template<typename _Head>
            struct next<_Head, false>
            {
                using type = Seq<Filtered...>;
            };

            using type = type_t<filter<Predicate, type_t<next<>>, Seq<Tail...>>>;
        };

        template<typename Predicate,
            template<typename...> class Seq, typename... Filtered>
        struct filter<Predicate, Seq<Filtered...>, Seq<>>
        {
            using type = Seq<Filtered...>;
        };
    }

    template<typename Function, typename Seed, typename Seq>
    struct foldl;

    template<typename Function, typename Seed, 
             template<typename...> class Seq, typename Head, typename... Tail>
    struct foldl<Function, Seed, Seq<Head, Tail...>>
    {
        using type = type_t<
            foldl<Function,
                  apply_t<Function, Seed, Head>,
                  Seq<Tail...>
            >
        >;
    };

    template<typename Function, typename Seed,
             template<typename...> class Seq>
    struct foldl<Function, Seed, Seq<>>
    {
        using type = Seed;
    };

    template<typename Function, typename Seed, typename Seq>
    struct foldr;

    template<typename Function, typename Seed,
        template<typename...> class Seq, typename Head, typename... Tail>
    struct foldr<Function, Seed, Seq<Head, Tail...>>
    {
        using type = apply_t<
            Function, Head,
            type_t<foldr<Function, Seed, Seq<Tail...>>>
        >;
    };

    template<typename Function, typename Seed,
        template<typename...> class Seq>
    struct foldr<Function, Seed, Seq<>>
    {
        using type = Seed;
    };

    template<typename Function, typename... Ts>
    using pack_fmap_t = type_t<pack_fmap<Function, Ts...>>;
    template<typename Function, typename Seq>
    using fmap_t = type_t<fmap<Function, Seq>>;

    template<typename Function, typename Seed, typename... Seq>
    using pack_foldl = foldl<Function, Seed, list<Seq...>>;
    template<typename Function, typename Seed, typename... Seq>
    using pack_foldl_t = type_t<pack_foldl<Function, Seed, Seq...>>;
    template<typename Function, typename Seed, typename Seq>
    using foldl_t = type_t<foldl<Function, Seed, Seq>>;

    template<typename Function, typename Seed, typename... Seq>
    using pack_foldr = foldr<Function, Seed, list<Seq...>>;
    template<typename Function, typename Seed, typename... Seq>
    using pack_foldr_t = type_t<pack_foldr<Function, Seed, Seq...>>;
    template<typename Function, typename Seed, typename Seq>
    using foldr_t = type_t<foldr<Function, Seed, Seq>>;

    template<typename Predicate, typename Seq>
    using filter = detail::filter<Predicate, apply_functor<Seq>, Seq>;
    template<typename Predicate, typename Seq>
    using filter_t = type_t<filter<Predicate, Seq>>;
    template<typename Predicate, typename... Seq>
    using pack_filter = detail::filter<Predicate, list<>, list<Seq...>>;
    template<typename Predicate, typename... Seq>
    using pack_filter_t = type_t<pack_filter<Predicate, Seq...>>;

    template<typename Bs>
    using any_of = foldl<or_, false_, Bs>;
    template<typename Bs>
    using any_of_t = foldl_t<or_, false_, Bs>;
    template<typename... Bs>
    using pack_any_of = pack_foldl<or_, false_, Bs...>;
    template<typename... Bs>
    using pack_any_of_t = pack_foldl_t<or_, false_, Bs...>;

    template<typename Bs>
    using any_of = foldl<or_, false_, Bs>;
    template<typename Bs>
    using any_of_t = foldl_t<or_, false_, Bs>;
    template<typename... Bs>
    using pack_any_of = pack_foldl<or_, false_, Bs...>;
    template<typename... Bs>
    using pack_any_of_t = pack_foldl_t<or_, false_, Bs...>;
}

}

#endif // SIPLASPLAS_UTILITY_META_H