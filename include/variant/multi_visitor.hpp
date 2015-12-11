
#ifndef CEPLUSPLUS_MULTI_VISITOR_HPP
#define CEPLUSPLUS_MULTI_VISITOR_HPP

#include <tuple>

namespace cpp
{
    namespace detail
    {
        template<
            typename Visitor,
            typename MatchedValues,
            typename HeadVariant,
            typename... TailVariants
        > class MultiVisitorProcessor
        {
        public:
            MultiVisitorProcessor(Visitor&& visitor, 
                                  MatchedValues&& matchedValues,
                                  HeadVariant&& headVariant,
                                  TailVariants&&... tailVariants) :
                _visitor{visitor},
                _matchedValues{matchedValues},
                _headVariant{headVariant},
                _tailVariants{tailVariants...}
            {}
            
            using result_type = std::decay_t<Visitor>;

        private:
            // Note parameters were not decayed, they conserve rl-qualifiers
            Visitor _visitor;
            
            MatchedValues _matchedValues;

            HeadVariant _headVariant;
            std::tuple<TailVariants...> _tailVariants;

            
        };
    }
}

#endif // CEPLUSPLUS_MULTI_VISITOR_HPP
