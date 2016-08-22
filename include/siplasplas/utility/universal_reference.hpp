#ifndef SIPLASPLAS_UTILITY_UNIVERSALREFERENCE_HPP
#define SIPLASPLAS_UTILITY_UNIVERSALREFERENCE_HPP

#include <type_traits>
#include <bitset>

namespace cpp
{

/**
 * \ingroup utility
 * \brief Implements a copyable wrapper of an universal reference
 *
 * This template and its specialization provides a common interface to store references
 * to lvalue and rvalues. UniversalReference has different spacializations for each value
 * category supported (reference to lvalue, const reference to lvalue, rvalue). Instances of
 * this types should be created through cpp::universalReference() function, which instances the
 * appropiate template given the input value category.
 *
 * If the input value is an lvalue (i.e. the function parameter is a reference to an lvalue),
 * UniversalReference specialization wrapping the reference is chosen. If the input value is an
 * rvalue (the function parameter is a reference to an rvalue), the value is moved into the
 * UniversalReference object and owned by it.
 *
 * This main template is the specialization for const lvalues
 */
template<typename T,
    bool IsLvalueReference,
    bool IsConst
>
class UniversalReference
{
public:
    using ValueType = T;

    UniversalReference(const ValueType& lvalueRef) :
        _ref{&lvalueRef}
    {}

    /**
     * \brief Returns a read only reference to the referenced object
     */
    const ValueType& get() const
    {
        return *_ref;
    }

private:
    const T* _ref;
};

/**
 * \ingroup utility
 * \brief UniversalReference spacialization for non-const lvalues
 *
 * See cpp::UniversalReference main template documentation for details
 */
template<typename T>
class UniversalReference<
    T,
    true,
    false
>
{
public:
    using ValueType = T;

    UniversalReference(ValueType& lvalueRef) :
        _ref{&lvalueRef}
    {}

    /**
     * \brief Returns a reference to the referenced object
     */
    ValueType& get() const
    {
        return *_ref;
    }
private:
    T* _ref;
};

/**
 * \ingroup utility
 * \brief UniversalReference spacialization for rvalues
 *
 * See cpp::UniversalReference main template documentation for details
 */
template<typename T>
class UniversalReference<
    T,
    false,
    false
>
{
public:
    using ValueType = T;

    UniversalReference(ValueType&& rvalueRef) :
        _object{std::move(rvalueRef)}
    {}

    /**
     * \brief Returns a reference to the referenced object
     */
    ValueType& get()
    {
        return _object;
    }

    /**
     * \brief Returns a reference to the referenced object
     */
    const ValueType& get() const
    {
        return _object;
    }

private:
    ValueType _object;
};

/**
 * \ingroup utility
 * \brief Creates an universal reference to the given value
 *
 * This function checks the value category of the given value and instances the
 * apropiate cpp::UniversalReference specialization. Note the returned type depends on the
 * value category. For an alternative with common a type, see cpp::typeerasure::AnyArg
 *
 * \returns A UniversalReference specialization instance as follows:
 *  - `UniversalReference<std::decay_t<T>, true, true>` if `value` type is deduced to a const
 *    reference to an lvalue.
 *  - `UniversalReference<std::decay_t<T>, true, false>` if `value` type is deduced to a non const
 *    reference to an lvalue.
 *  - `UniversalReference<std::decay_t<T>, false, false>` if `value` type is deduced to an rvalue reference 
 */
template<typename T>
auto universalReference(T&& value) ->
UniversalReference<
    std::decay_t<T>,
    std::is_lvalue_reference<T>::value,
    std::is_const<std::remove_reference_t<T>>::value
>
{
    return { std::forward<T>(value) };
}

}

#endif // SIPLASPLAS_UTILITY_UNIVERSALREFERENCE_HPP
