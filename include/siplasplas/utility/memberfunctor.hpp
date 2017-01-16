#ifndef SIPLASPLAS_UTILITY_MEMBERFUNCTOR_HPP
#define SIPALSPLAS_UTILITY_MEMBERFUNCTOR_HPP

#include "meta.hpp"
#include <utility>

namespace cpp
{

/**
 * \ingroup utility
 * \brief Checks if a type is a candidate for the Empty Base Optimization (EBO)
 *
 * This trait checks if the compiler would perform EBO when inheriting from
 * the give type. This could be a hint to decide whether to store a T member
 * by inheritance or by aggregation to save some space.
 *
 * Note that any compliant compiler would perform this optimization on any
 * empty type, so using standard `std::is_empty` trait should be enough.
 *
 * \tparam T Type to check EBO-ness
 */
template<typename T>
using IsEboCandidate = ::cpp::meta::bool_<
    (sizeof(::cpp::meta::inherit<T, ::cpp::meta::aggregate<int>>) == sizeof(int))
>;

/**
 * \ingroup utility
 * \brief Implements optimal-storage for a functor that will be used
 * as member of a class
 *
 * This class provides optimal storage for a functor instance that's
 * supposed to be used as member of a class. The class is designed to be
 * inherited from the user class which the functor member will belong to.
 *
 * ``` cpp
 * template<typename T, typename Alloc = std::allocator<T>>
 * class Vector : MemberFunction<Alloc>
 * {
 *     using Allocator = MemberFunction<Alloc>;
 * };
 * ```
 *
 * \tparam F Functor type
 */
template<typename F, bool Ebo = IsEboCandidate<F>::value>
class MemberFunctor : private F
{
protected:
    /**
     * \brief Initializes a member functor with the given arguments
     */
    template<typename... Args>
    constexpr MemberFunctor(Args&&... args) :
        F{std::forward<Args>(args)...}
    {}

    /**
     * \brief Invokes the member functor with the given
     * arguments
     */
    template<typename... Args>
    constexpr decltype(auto) invoke(Args&&... args) const
    {
        return F::operator()(std::forward<Args>(args)...);
    }

    /**
     * \brief Invokes the member functor with the given
     * arguments
     */
    template<typename... Args>
    constexpr decltype(auto) invoke(Args&&... args)
    {
        return F::operator()(std::forward<Args>(args)...);
    }

    /**
     * \brief Returns a const reference to the functor object
     */
    constexpr const F& get() const
    {
        return *static_cast<const F*>(this);
    }

    /**
     * \brief Returns a reference to the member functor
     */
    constexpr F& get()
    {
        return *static_cast<F*>(this);
    }
};

template<typename F>
class MemberFunctor<F, false>
{
protected:
    template<typename... Args>
    constexpr MemberFunctor(Args&&... args) :
        _functor{std::forward<Args>(args)...}
    {}

    template<typename... Args>
    constexpr decltype(auto) invoke(Args&&... args) const
    {
        return _functor(std::forward<Args>(args)...);
    }

    template<typename... Args>
    constexpr decltype(auto) invoke(Args&&... args)
    {
        return _functor(std::forward<Args>(args)...);
    }

    constexpr const F& get() const
    {
        return _functor;
    }

    constexpr F& get()
    {
        return _functor;
    }

private:
    F _functor;
};

}

#endif // SIPLASPLAS_UTILITY_MEMBERFUNCTOR_HPP
