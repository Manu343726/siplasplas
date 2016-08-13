#ifndef SIPLASPLAS_TYPEERASURE_FEATURESGROUP_HPP
#define SIPLASPLAS_TYPEERASURE_FEATURESGROUP_HPP

namespace cpp
{

namespace features
{

namespace detail
{

template<typename Feature>
class CompressedFeature : public Feature
{
public:
    template<typename... Args>
    static decltype(auto) invokeFeature(Feature*, Args&&... args)
    {
        return Feature::apply(std::forward<Args>(args)...);
    }
};

}

template<typename... Features>>
class FeaturesGroup : public CompressedFeature<Features>...
{
public:
    template<typename Feature, typename... Args>>
    static decltype(auto) apply(Args&&... args)
    {
        return invokeFeature(static_cast<Feature*>(nullptr), std::forward<Args>(args)...)
    }
};

}

}

#endif // SIPLASPLAS_TYPEERASURE_FEATURESGROUP_HPP
