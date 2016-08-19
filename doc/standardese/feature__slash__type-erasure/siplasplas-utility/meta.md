---
layout: standardese-doc
---

# Header file `meta.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_META_H 

#include <type_traits>

#include <cstdint>

namespace cpp
{
    namespace meta
    {
        template <typename B, typename ... Bs>
        struct assert<B, Bs...>;
        
        template <>
        struct assert<>;
        
        template <typename T>
        struct identity;
        
        template <typename MetafunctionClass, template <typename> typename Seq, typename ... Ts>
        struct sequence_apply<MetafunctionClass, Seq<Ts...>;
        
        using true_ = bool_<true>;
        
        using false_ = bool_<false>;
        
        template <typename T, typename = void>
        struct is_integral;
        
        template <typename T>
        struct is_integral<T, void_t<decltype(T::value)>;
        
        template <typename T, typename = assert<is_integral<T>>>
        constexpr decltype(T::value) value();
        
        template <template <typename> typename Function>
        struct defer;
        
        struct and_;
        
        struct or_;
        
        struct add_;
        
        template <typename ... Ts>
        struct list;
        
        template <char ... Chars>
        class StringToArray<list<std::integral_constant<char, Chars>...>;
        
        template <template <typename> typename Seq, typename ... Ts>
        struct functor<Seq<Ts...>;
        
        template <template <typename> typename Seq, typename ... Lhs, typename ... Rhs>
        struct cat<Seq<Lhs...>, Seq<Rhs...>;
        
        namespace detail
        {
            template <template <typename> typename Seq, std::size_t Index, typename ... Left, typename Head, typename ... Tail>
            struct split<Seq<Left...>, Index, Seq<Head, Tail...>;
            
            template <template <typename> typename Seq, typename ... Left, typename Head, typename ... Tail>
            struct split<Seq<Left...>, 0, Seq<Head, Tail...>;
        }
        
        template <std::size_t Index, template <typename> typename Seq, typename ... Ts>
        struct split<Index, Seq<Ts...>;
        
        template <typename T, template <typename> typename Seq, typename ... Ts>
        struct prepend<T, Seq<Ts...>;
        
        template <typename T, template <typename> typename Seq, typename ... Ts>
        struct append<T, Seq<Ts...>;
        
        template <typename Key, typename Value>
        struct pair;
        
        template <typename ... Ts>
        struct inherit;
        
        template <typename ... Ts>
        struct inherit<list<Ts...>;
        
        template <typename ... Keys, typename ... Values>
        struct map<pair<Keys, Values>...>;
        
        template <typename Function, typename ... Ts>
        struct pack_fmap;
        
        template <typename Function, template <typename> typename Seq, typename ... Ts>
        struct fmap<Function, Seq<Ts...>;
        
        namespace detail
        {
            template <typename Predicate, template <typename> typename Seq, typename ... Filtered, typename Head, typename ... Tail>
            struct filter<Predicate, Seq<Filtered...>, Seq<Head, Tail...>;
            
            template <typename Predicate, template <typename> typename Seq, typename ... Filtered>
            struct filter<Predicate, Seq<Filtered...>, Seq<>;
        }
        
        template <typename Function, typename Seed, template <typename> typename Seq, typename Head, typename ... Tail>
        struct foldl<Function, Seed, Seq<Head, Tail...>;
        
        template <typename Function, typename Seed, template <typename> typename Seq>
        struct foldl<Function, Seed, Seq<>;
        
        template <typename Function, typename Seed, template <typename> typename Seq, typename Head, typename ... Tail>
        struct foldr<Function, Seed, Seq<Head, Tail...>;
        
        template <typename Function, typename Seed, template <typename> typename Seq>
        struct foldr<Function, Seed, Seq<>;
        
        namespace detail
        {
            template <template <typename> typename Seq, std::size_t N>
            struct make_index_sequence;
            
            template <template <typename> typename Seq>
            struct make_index_sequence<Seq, 1>;
            
            template <template <typename> typename Seq>
            struct make_index_sequence<Seq, 0>;
        }
        
        template <template <typename> typename Seq, typename ... Ts>
        struct to_index_sequence<Seq<Ts...>;
        
        template <template <typename> typename Zipper, typename ... Seqs>
        struct zip;
        
        template <template <typename> typename Function, typename ... Args>
        struct bind;
    }
}
```
