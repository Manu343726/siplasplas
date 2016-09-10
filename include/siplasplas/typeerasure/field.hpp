#ifndef SIPLASPLAS_TYPEERASURE_FIELD_HPP
#define SIPLASPLAS_TYPEERASURE_FIELD_HPP

#include "function.hpp"

namespace cpp
{

namespace typeerasure
{

/**
 * \ingroup type-erasure
 * \brief Stores a type erased member object pointer
 *
 * Field class can store pointers to member objects of any class
 * and type, suitable to be stored in an homogeneous context (Such as a vector).
 * It also provides methods to access the type-erased pointer and access the
 * member object of a given object
 *
 * \tparam Storage Storage for the member function pointer
 * \tparam ObjectArgStorage Storage for the object argument of a get() call
 */
template<typename Storage, typename ObjectArgStorage = Storage>
class Field
{
public:
    Field() = default;

    /**
     * \brief Checks if the field is empty
     * Empty is defined as having no member object pointer assigned to
     * the Field object. See SimpleAny::empty()
     */
    bool empty() const
    {
        return _invoke.empty();
    }

    /**
     * \brief Returns a reference to the member object pointer stored by
     * the Field object
     *
     * The behavior is undefined if the Field object is empty
     *
     * \tparam T Member object pointer. The behavior is undefined if is not the type of
     * the member object pointer stored by the Field object
     */
    /**
     * \brief Returns the kind of the member object pointer
     */
    cpp::FunctionKind kind() const
    {
        return _invoke.kind();
    }

    /**
     * \brief Initializes a Field with a given member object pointer
     */
    template<typename T, typename = std::enable_if_t<
        std::is_member_object_pointer<std::decay_t<T>>::value
    >>
    Field(T&& field) :
        _invoke{std::forward<T>(field)}
    {}

    /**
     * \brief Returns the value of the member object of a given
     * object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \returns A type-erased reference to the member object in the given object
     */
    template<typename Object>
    decltype(auto) get(const Object& object) const
    {
        return _invoke(object);
    }

    /**
     * \brief Returns the value of the member object of a given
     * object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \tparam T Type of the member object. The behavior is undefined if this is not
     * the type of the member object
     *
     * \returns A reference to the member object in the given object
     */
    template<typename T, typename Object>
    decltype(auto) getAs(const Object& object) const
    {
        return get(object).template get<T>();
    }

    /**
     * \brief Returns the value of the member object of a given
     * object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \returns A type-erased reference to the member object in the given object
     */
    template<typename Object>
    /**
     * \brief Returns a reference to the member object pointer stored by
     * the Field object
     *
     * The behavior is undefined if the Field object is empty
     *
     * \tparam T Member object pointer. The behavior is undefined if is not the type of
     * the member object pointer stored by the Field object
     */
    decltype(auto) get(Object& object) const
    {
        return _invoke(object);
    }

    /**
     * \brief Returns the value of the member object of a given
     * object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \tparam T Type of the member object. The behavior is undefined if this is not
     * the type of the member object
     *
     * \returns A reference to the member object in the given object
     */
    template<typename T, typename Object>
    decltype(auto) getAs(Object& object) const
    {
        return get(object).template get<T>();
    }

    /**
     * \brief Returns the value of the member object of a given
     * type-erased object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \returns A type-erased reference to the member object in the given type-erased object
     */
    template<typename Storage_>
    decltype(auto) get(const cpp::SimpleAny<Storage_>& object) const
    {
        return _invoke(object.getReference());
    }

    /**
     * \brief Returns the value of the member object of a given
     * type erased object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \tparam T Type of the member object. The behavior is undefined if this is not
     * the type of the member object
     *
     * \returns A reference to the member object in the given type-erased object
     */
    template<typename T, typename Storage_>
    decltype(auto) getAs(const cpp::SimpleAny<Storage_>& object) const
    {
        return get(object).template get<T>();
    }

    /**
     * \brief Returns the value of the member object of a given
     * type-erased object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \returns A type-erased reference to the member object in the given type-erased object
     */
    template<typename Storage_>
    decltype(auto) get(cpp::SimpleAny<Storage_>& object) const
    {
        return _invoke(object.getReference());
    }

    /**
     * \brief Returns the value of the member object of a given
     * type-erased object reference
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \returns A type-erased reference to the member object in the given type-erased object
     */
    decltype(auto) get(cpp::ReferenceSimpleAny& object) const
    {
        return _invoke(object);
    }

    /**
     * \brief Returns the value of the member object of a given
     * type-erased object reference
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \returns A type-erased reference to the member object in the given type-erased object
     */
    decltype(auto) get(cpp::ConstReferenceSimpleAny& object) const
    {
        return _invoke(object);
    }
    /**
     * \brief Returns the value of the member object of a given
     * type erased object
     *
     * This function accesses to the member object referenced by the Field object on
     * a given object. The behavior is undefined if the given object is not of the same class
     * the stored member object pointer belongs to. Also, the behavior is undefined if
     * the Field object is empty.
     *
     * \tparam T Type of the member object. The behavior is undefined if this is not
     * the type of the member object
     *
     * \returns A reference to the member object in the given type-erased object
     */
    template<typename T, typename Storage_>
    decltype(auto) getAs(cpp::SimpleAny<Storage_>& object) const
    {
        return get(object).template get<T>();
    }

    /**
     * \brief Assigns a new member object pointer to the Field object
     */
    template<typename T>
    std::enable_if_t<
        std::is_member_object_pointer<std::decay_t<T>>::value,
        Field&
    >
    operator=(T&& field)
    {
        _invoke = std::forward<T>(field);
        return *this;
    }

    /**
     * \brief Returns a reference to the member object pointer stored by
     * the Field object
     *
     * The behavior is undefined if the Field object is empty
     *
     * \tparam T Member object pointer. The behavior is undefined if is not the type of
     * the member object pointer stored by the Field object
     */
    template<typename T>
    const T& get() const
    {
        return _invoke.template get<T>();
    }

    /**
     * \brief Returns a reference to the member object pointer stored by
     * the Field object
     *
     * The behavior is undefined if the Field object is empty
     *
     * \tparam T Member object pointer. The behavior is undefined if is not the type of
     * the member object pointer stored by the Field object
     */
    template<typename T>
    T& get()
    {
        return _invoke.template get<T>();
    }

private:
    cpp::typeerasure::Function<Storage, ObjectArgStorage, cpp::NonOwningStorage> _invoke;
};


/**
 * \ingroup type-erasure
 * \brief A field with 8 byte fixed-size storage
 */
using Field8  = Field<DeadPoolStorage<8>>;

/**
 * \ingroup type-erasure
 * \brief A field with 16 byte fixed-size storage
 */
using Field16 = Field<DeadPoolStorage<16>>;

/**
 * \ingroup type-erasure
 * \brief A field with 32 byte fixed-size storage
 */
using Field32 = Field<DeadPoolStorage<32>>;

/**
 * \ingroup type-erasure
 * \brief A field with 64 byte fixed-size storage
 */
using Field64 = Field<DeadPoolStorage<64>>;

}

}

#endif // SIPLASPLAS_TYPEERASURE_FIELD_HPP
