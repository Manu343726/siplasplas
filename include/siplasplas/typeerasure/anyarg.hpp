#ifndef SIPLASPLAS_TYPEERASURE_ANYARG_HPP
#define SIPLASPLAS_TYPEERASURE_ANYARG_HPP

#include "simpleany.hpp"
#include <siplasplas/utility/universal_reference.hpp>

namespace cpp
{

/**
 * \ingroup type-erasure
 * \brief Represents a type erased function call argument
 *
 * cpp::SimpleAny and cpp::Any classes represent type erased values
 * that are **owned** by the any object, except explicitly stated
 * by using `Reference` family of specializations. That means that
 * when a normal any class is initialized with an lvalue, the value
 * **is copied into the any object**. While this works for heterogeneous
 * containers, that semantics doesn't fit the semantics expected on a
 * function call, where parameters taking references alias existing objects.
 *
 * AnyArg implements the semantics of a function call following two rules:
 *
 *  - If the initializer value is an lvalue, AnyArg instance is a reference
 *    to the value. AnyArg objects are copyable.
 *
 *  - If the initializer value is an rvalue, the initializer value is moved into
 *    the AnyArg. The AnyArg object owns the value. If the moved in value is not
 *    copyable, an exception is thrown when performing AnyArg copy. See cpp::features::Copyable.
 */
class AnyArg
{
public:
    /**
     * \brief Initializes an AnyArg object with a given value
     */
    template<typename T>
    AnyArg(T&& value) :
        _universalRef{cpp::universalReference(std::forward<T>(value))}
    {
        _typeProperties[0] = std::is_lvalue_reference<T>::value;
        _typeProperties[1] = std::is_const<std::remove_reference_t<T>>::value;
    }

    /**
     * \brief Returns a const reference to the argument object
     *
     * \tparam T Type of the argument (cv qualifiers and references are ignored). The behavior is
     * undefined if T is not the original type of the argument
     *
     * \returns A const reference to the argument
     */
    template<typename T>
    const T& get() const
    {
        if(isRvalue())
        {
            return _universalRef.get<cpp::UniversalReference<std::decay_t<T>, false, false>>().get();
        }
        else if(isConst())
        {
            return _universalRef.get<cpp::UniversalReference<std::decay_t<T>, true, true>>().get();
        }
        else
        {
            return _universalRef.get<cpp::UniversalReference<std::decay_t<T>, true, false>>().get();
        }
    }

    /**
     * \brief Returns a reference to the argument object
     *
     * \tparam T Type of the argument (cv qualifiers and references are ignored). The behavior is
     * undefined if T is not the original type of the argument
     * The behavior is also undefined if this get() overload is called and the argument is
     * const lvalue reference (See isConst() and isLvalue())
     *
     * \returns A reference to the argument
     */
    template<typename T>
    T& get()
    {
        if(isRvalue())
        {
            return _universalRef.get<cpp::UniversalReference<std::decay_t<T>, false, false>>().get();
        }
        else if(isConst())
        {
            throw std::runtime_error{"Cannot get a non-const reference to a const argument"};
        }
        else
        {
            return _universalRef.get<cpp::UniversalReference<std::decay_t<T>, true, false>>().get();
        }
    }

    /**
     * \brief Checks if the argument was an rvalue
     */
    bool isRvalue() const
    {
        return !_typeProperties[0];
    }

    /**
     * \brief Checks if the argument was an lvalue
     */
    bool isLvalue() const
    {
        return _typeProperties[0];
    }

    /**
     * \brief Checks if the argument was const
     */
    bool isConst() const
    {
        return _typeProperties[1];
    }

private:
    cpp::SimpleAny32 _universalRef;
    std::bitset<2> _typeProperties;
};

}

#endif // SIPLASPLAS_TYPEERASURE_ANYARG_HPP
