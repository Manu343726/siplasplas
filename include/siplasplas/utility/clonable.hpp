#ifndef SIPLASPLAS_UTILITY_CLONABLE_HPP
#define SIPLASPLAS_UTILITY_CLONABLE_HPP

#include <type_traits>

namespace cpp
{

class Clonable
{
public:
    virtual ~Clonable() = default;
    virtual Clonable* clone() const = 0;
};

template<typename Class, typename ClonableInterface = Clonable>
class DefaultClone : public ClonableInterface
{
public:
    template<typename... Args>
    DefaultClone(Args&&... args) :
        ClonableInterface{std::forward<Args>(args)...}
    {}

    ClonableInterface* clone() const override
    {
        return new Class(*static_cast<const Class*>(this));
    }
};

}

#endif // SIPLASPLAS_UTILITY_CLONABLE_HPP
