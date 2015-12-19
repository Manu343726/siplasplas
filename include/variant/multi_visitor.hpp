
#ifndef CEPLUSPLUS_MULTI_VISITOR_HPP
#define CEPLUSPLUS_MULTI_VISITOR_HPP

#include <tuple>

#include "variant/visitor.hpp"

namespace cpp
{
    namespace detail
    {
        template<typename Head, typename... Tail, std::size_t... Is>
        auto tuple_tail(const std::tuple<Head, Tail...>& tuple, std::index_sequence<Is...>)
        {
            return std::make_tuple(std::forward<std::tuple_element_t<Is+1, std::tuple<Head, Tail>>>(std::get<Is+1>(tuple))...);
        }

        template<typename Head, typename... Tail>
        auto tuple_tail(const std::tuple<Head, Tail...>& tuple)
        {
            return tuple_tail(tuple, std::index_sequence_for<Tail...>{});
        }

        template<
            typename Visitor,
            typename MatchedValues,
            typename Variants
        > class MultiVisitorProcessor;

        template<
            typename Visitor,
            typename MatchedValues,
            typename Variants
        > MultiVisitorProcessor<Visitor,
                                MatchedValues,
                                Variants>
        make_visitor_processor(Visitor&& visitor, 
                               MatchedValues&& matchedValues,
                               Variants&& variants)
        {
            return { std::forward<Visitor>(visitor),
                     std::forward<MatchedValues>(matchedValues),
                     std::forward<Variants>(variants) };
        }

        template<
            typename Visitor,
            typename MatchedValues,
            typename Variant,
            typename...  Tail
        > class MultiVisitorProcessor<Visitor, MatchedValues, std::tuple<Variant, Tail...>>
        {
        public:
            MultiVisitorProcessor(Visitor&& visitor, 
                                  MatchedValues&& matchedValues,
                                  std::tuple<Variant, Tail...>&& variants) :
               _visitor{visitor},
                _matchedValues{matchedValues},
                _variants{variants}
            {}
            
            using ResultType = typename std::decay_t<Visitor>::ResultType;

            template<typename T>
            auto next_visitor(T&& value)
            {
                return make_visitor_processor(
                    std::forward<Visitor>(_visitor),
                    std::tuple_cat(_matchedValues, std::forward_as_tuple(std::forward<T>(value))),
                    tuple_tail(_variants)
                );
            }

            template<typename T>
            ResultType operator()(T&& value)
            {
                return std::forward<Variant>(std::get<0>(_variants)).template visit<ResultType>(next_visitor(std::forward<T>(value)));
            }
        private:
            // Note parameters were not decayed, they conserve rl-qualifiers
            Visitor _visitor; 
            MatchedValues _matchedValues;
            std::tuple<Variant, Tail...> _variants; 
        };

        template<
            typename Visitor,
            typename MatchedValues
        > class MultiVisitorProcessor<Visitor, MatchedValues, std::tuple<>>
        {
        public:
            MultiVisitorProcessor(Visitor&& visitor, 
                                  MatchedValues&& matchedValues,
                                  std::tuple<>&& variant) :
               _visitor{visitor},
               _matchedValues{matchedValues}
            {}
            
            using ResultType = typename std::decay_t<Visitor>::ResultType;

            template<typename T>
            ResultType operator()(T&& value)
            {
                return call_multivisitor(std::forward<T>(value), 
                                         std::make_index_sequence<
                                             std::tuple_size<MatchedValues>::value
                                         >{}
                );
            }

            template<typename T, std::size_t... Is>
            ResultType call_multivisitor(T&& value, std::index_sequence<Is...>)
            {
                return std::forward<Visitor>(_visitor)(
                    std::forward<std::tuple_element_t<Is, MatchedValues>>(std::get<Is>(_matchedValues))...,
                    std::forward<T>(value)
                );
            }
        private:
            // Note parameters were not decayed, they conserve rl-qualifiers
            Visitor _visitor; 
            MatchedValues _matchedValues;
        };
    }

    template<typename T, typename... Visitor>
    auto multi_visitor(Visitor&&... visitor)
    {
        return [&visitor...](auto&& variant, auto&&... variants)
        {
            auto processor = ::cpp::detail::make_visitor_processor(
                cpp::visitor<T>(std::forward<Visitor>(visitor)...),
                std::make_tuple(),
                std::forward_as_tuple(std::forward<decltype(variants)>(variants)...)
            );

            return std::forward<decltype(variant)>(variant).template visit<T>(processor);
        };
    }
}

#endif // CEPLUSPLUS_MULTI_VISITOR_HPP
