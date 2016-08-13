#ifndef SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
#define SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP

#include "typeinfo.hpp"
#include "anystorage/fixedsize.hpp"
#include "anystorage/nonowning.hpp"
#include "logger.hpp"
#include <siplasplas/utility/assert.hpp>
#include <cstdint>

namespace cpp
{

/**
 * \ingroup type-erasure
 * \brief Implements a type-erased value container with minimal
 * value semantics requirements
 *
 * The template SimpleAny implements a type-erased value container, that is,
 * a type that can hold values of different types at runtime.
 *
 * SimpleAny assumes that types hosted on it satisfy the CopyConstructible,
 * MoveConstructible, CopyAssignable, MoveAssignable, and Destructible. If any of those
 * concepts are not satisfied, an exception would be thrown if the corresponding feature is used.
 * This means you can use non-satisfying types as long as the operation is not needed (Invoked). For example:
 *
 * ``` cpp
 * cpp::SimpleAny<64> any{NoCopyAssignable()}: // Ok, NoCopyAssignable is move constructible
 * auto any2 = cpp::SimpleAny<64>::create<NoCopyAssignable>(); // Ok, NoCopyAssignable is default constructible
 * any2 = any; // EXCEPTION THROWN: NoCopyAssignable is not copy assignable (Noooo, really?)
 * any2 = std::move(any); // Ok, NoCopyAssignable is move assignable
 * ```
 *
 * Note this behavior is undefined and may be disabled in release builds. In general,
 * **using semantics not supporeted by the hosted type has undefined behavior**.
 *
 * \tparam Storage Storage policy. See any storage
 */
template<typename Storage>
class SimpleAny : protected Storage
{
public:

    /**
     * \brief Constructs a SimpleAny with an in-place constructed value of
     * type T
     *
     * The value is constructed directly on the any storage, no extra copy operations are done.
     * Arguments are passed as-is to the object constructor.
     *
     * \tparam T Type of the value to be constructed.
     * \param args Constructor arguments
     *
     * \returns A SimpleAny instance hosting an object of type T
     */
    template<typename T, typename... Args>
    static SimpleAny create(Args&&... args)
    {
        return SimpleAny{meta::identity<T>(), std::forward<Args>(args)...};
    }

    /**
     * \brief Constructs an any of type T from an lvalue of type T
     *
     * A copy is done from the T lvalue argument to the any storage
     *
     * \param value A value of type T to store in the any
     */
    template<typename T>
    SimpleAny(const T& value) :
        _typeInfo{TypeInfo::get<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(Storage::template objectFitsInStorage<T>());
        _typeInfo.copyConstruct(Storage::storage(_typeInfo.alignment()), &value);
    }

    SimpleAny(const SimpleAny& other) :
        _typeInfo{other._typeInfo}
    {
        _typeInfo.copyConstruct(Storage::storage(_typeInfo.alignment()), other.storage());
    }

    SimpleAny(SimpleAny&& other) :
        _typeInfo{other._typeInfo}
    {
        _typeInfo.moveConstruct(Storage::storage(_typeInfo.alignment()), other.storage());
    }

    /**
     * \brief Checks if the any has a value of type T
     *
     * \returns True if the hosted type is **exactly** T, false otherwise
     */
    template<typename T>
    bool hasType() const
    {
        return TypeInfo::get<T>() == _typeInfo;
    }

    /**
     * \brief Returns a readonly reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    const T& get() const
    {
        SIPLASPLAS_ASSERT_TRUE(hasType<T>());
        return *reinterpret_cast<const T*>(Storage::storage(_typeInfo.alignment()));
    }

    /**
     * \brief Returns a eference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    T& get()
    {
        SIPLASPLAS_ASSERT_TRUE(hasType<T>());
        return *reinterpret_cast<T*>(Storage::storage(_typeInfo.alignment()));
    }

    /**
     * \brief Returns the type information of the hosted type
     */
    TypeInfo typeInfo() const
    {
        return _typeInfo;
    }

    /**
     * \brief Assigns a value of type T
     *
     * If the current hosted type is T, performs a copy assignment of \p value
     * into the hosted object. Else, the destructor of the hosted object is invoked
     * and the value is copy-constructed into the storage.
     *
     * \param value Value to be assigned to the any
     * \returns A reference to `*this`
     */
    template<typename T>
    SimpleAny& operator=(const T& value)
    {
        if(!hasType<T>())
        {
            _typeInfo.destroy(Storage::storage(_typeInfo.alignment()));
            _typeInfo = TypeInfo::get<T>();
            SIPLASPLAS_ASSERT_TRUE(Storage::template objectFitsInStorage<T>());
            _typeInfo.copyConstruct(Storage::storage(_typeInfo.alignment()), &value);
        }
        else
        {
            _typeInfo.copyAssign(Storage::storage(_typeInfo.alignment()), &value);
        }

        return *this;
    }

    SimpleAny& operator=(const SimpleAny& other)
    {
        if(!sameType(*this, other))
        {
            _typeInfo.destroy(Storage::storage(_typeInfo.alignment()));
            _typeInfo = other._typeInfo;
            _typeInfo.copyConstruct(Storage::storage(_typeInfo.alignment()), other.storage());
        }
        else
        {
            _typeInfo.copyAssign(Storage::storage(_typeInfo.alignment()), other.storage());
        }

        return *this;
    }

    SimpleAny& operator=(SimpleAny&& other)
    {
        if(!sameType(*this, other))
        {
            _typeInfo.destroy(Storage::storage(_typeInfo.alignment()));
            _typeInfo = other._typeInfo;
            _typeInfo.moveConstruct(Storage::storage(_typeInfo.alignment()), other.storage());
        }
        else
        {
            _typeInfo.moveAssign(Storage::storage(_typeInfo.alignment()), other.storage());
        }

        return *this;
    }

    ~SimpleAny()
    {
        _typeInfo.destroy(Storage::storage(_typeInfo.alignment()));
    }

private:
    TypeInfo _typeInfo;

    template<typename T, typename... Args>
    SimpleAny(meta::identity<T>, Args&&... args) :
        _typeInfo{TypeInfo::get<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(Storage::template objectFitsInStorage<T>());
        features::Constructible::apply<T>(Storage::storage(_typeInfo.alignment()), std::forward<Args>(args)...);
    }
};


/**
 * \brief Checks if two SimpleAny objects host values of the same type
 *
 * \returns True if both hosted types are **exactly equal** i
 * (i.e. `std::is_same<hosted type of lhs, hosted type of rhs>::value` would yield true).
 */
template<typename LhsStorage, typename RhsStorage>
bool sameType(const SimpleAny<LhsStorage>& lhs, const SimpleAny<RhsStorage>& rhs)
{
    return lhs.typeInfo() == rhs.typeInfo();
}

/**
 * \brief cpp::SimpleAny specialization for non-owning const references to existing objects.
 * See cpp::ConstNonOwningStorage
 */
template<>
class SimpleAny<ConstNonOwningStorage> : protected ConstNonOwningStorage
{
public:
    template<typename T>
    SimpleAny(const T& value) :
        ConstNonOwningStorage(&value),
        _typeInfo{TypeInfo::get<T>()}
    {}

    /**
     * \brief Creates a const reference SimpleAny referencing the
     * given object of type T
     */
    template<typename T>
    static SimpleAny create(const T& value)
    {
        return SimpleAny(value);
    }

    /**
     * \brief Checks if the any has a value of type T
     *
     * \returns True if the hosted type is **exactly** T, false otherwise
     */
    template<typename T>
    bool hasType() const
    {
        return TypeInfo::get<T>() == _typeInfo;
    }

    /**
     * \brief Returns a readonly reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    const T& get() const
    {
        SIPLASPLAS_ASSERT_TRUE(hasType<T>());
        return *reinterpret_cast<const T*>(ConstNonOwningStorage::storage(_typeInfo.alignment()));
    }

    /**
     * \brief Returns the type information of the hosted type
     */
    TypeInfo typeInfo() const
    {
        return _typeInfo;
    }

    /**
     * \brief Assigns a value of type T
     *
     * If the current hosted type is T, performs a copy assignment of \p value
     * into the hosted object. Else, the hosted reference is rebinded
     * to point to the new value.
     *
     * \param value Value to be assigned to the any
     * \returns A reference to `*this`
     */
    template<typename T>
    SimpleAny& operator=(const T& value)
    {
        if(!hasType<T>())
        {
            _typeInfo = TypeInfo::get<T>();
            ConstNonOwningStorage::rebind(&value);
        }
        else
        {
            _typeInfo.copyAssign(ConstNonOwningStorage::storage(_typeInfo.alignment()), &value);
        }

        return *this;
    }

private:
    TypeInfo _typeInfo;
};

/**
 * \brief cpp::SimpleAny specialization for non-owning references to existing objects.
 * See cpp::NonOwningStorage
 */
template<>
class SimpleAny<NonOwningStorage> : protected NonOwningStorage
{
public:
    template<typename T>
    SimpleAny(T& value) :
        NonOwningStorage(const_cast<std::decay_t<T>*>(&value)),
        _typeInfo{TypeInfo::get<std::decay_t<T>>()}
    {}

    /**
     * \brief Creates a const reference SimpleAny referencing the
     * given object of type T
     */
    template<typename T>
    static SimpleAny create(const T& value)
    {
        return SimpleAny(value);
    }

    /**
     * \brief Checks if the any has a value of type T
     *
     * \returns True if the hosted type is **exactly** T, false otherwise
     */
    template<typename T>
    bool hasType() const
    {
        return TypeInfo::get<T>() == _typeInfo;
    }

    /**
     * \brief Returns a readonly reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    const T& get() const
    {
        SIPLASPLAS_ASSERT_TRUE(hasType<T>());
        return *reinterpret_cast<const T*>(NonOwningStorage::storage(_typeInfo.alignment()));
    }

    /**
     * \brief Returns a reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    T& get()
    {
        SIPLASPLAS_ASSERT_TRUE(hasType<T>());
        return *reinterpret_cast<T*>(NonOwningStorage::storage(_typeInfo.alignment()));
    }

    /**
     * \brief Returns the type information of the hosted type
     */
    TypeInfo typeInfo() const
    {
        return _typeInfo;
    }

    /**
     * \brief Assigns a value of type T
     *
     * If the current hosted type is T, performs a copy assignment of \p value
     * into the hosted object. Else, the hosted reference is rebinded
     * to point to the new value.
     *
     * \param value Value to be assigned to the any
     * \returns A reference to `*this`
     */
    template<typename T>
    SimpleAny& operator=(const T& value)
    {
        if(!hasType<T>())
        {
            _typeInfo = TypeInfo::get<T>();
            NonOwningStorage::rebind(&value);
        }
        else
        {
            _typeInfo.copyAssign(NonOwningStorage::storage(_typeInfo.alignment()), &value);
        }

        return *this;
    }

private:
    TypeInfo _typeInfo;
};

/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 8 byte fixed-size storage
 */
using SimpleAny8  = SimpleAny<FixedSizeStorage<8>>;

/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 16 byte fixed-size storage
 */
using SimpleAny16 = SimpleAny<FixedSizeStorage<16>>;

/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 32 byte fixed-size storage
 */
using SimpleAny32 = SimpleAny<FixedSizeStorage<32>>;

/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 64 byte fixed-size storage
 */
using SimpleAny64 = SimpleAny<FixedSizeStorage<64>>;

/**
 * \ingroup type-erasure
 * \brief A non-owning SimpleAny that (const) references an existing object
 */
using ConstReferenceAny = SimpleAny<ConstNonOwningStorage>;

/**
 * \ingroup type-erasure
 * \brief A non-owning SimpleAny that references an existing object
 */
using ReferenceAny = SimpleAny<NonOwningStorage>;


}

#endif // SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
