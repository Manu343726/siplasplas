#ifndef SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
#define SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP

#include "typeinfo.hpp"
#include "anystorage/deadpool.hpp"
#include "anystorage/nonowning.hpp"
#include "logger.hpp"
#include <siplasplas/utility/assert.hpp>
#include <cstdint>

namespace cpp
{

template<typename Storage>
class SimpleAny;

template<>
class SimpleAny<cpp::NonOwningStorage>;

template<>
class SimpleAny<cpp::ConstNonOwningStorage>;


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
class SimpleAny<ConstNonOwningStorage> : public ConstNonOwningStorage
{
public:
    struct EmptyTag {};

    SimpleAny() :
        ConstNonOwningStorage{nullptr},
        _typeInfo{cpp::typeerasure::TypeInfo::get<EmptyTag>()}
    {}

    /**
     * \brief Checks whether the any has an object hosted in or
     * if is empty
     *
     * \returns True if the any is empty (There's no hosted object), false
     * instead
     */
    bool empty() const
    {
        return hasType<EmptyTag>();
    }

    template<typename T>
    SimpleAny(const T& value) :
        ConstNonOwningStorage(&value),
        _typeInfo{cpp::typeerasure::TypeInfo::get<T>()}
    {}

    SimpleAny(const void* object, const cpp::typeerasure::TypeInfo& typeInfo) :
        ConstNonOwningStorage{object},
        _typeInfo{typeInfo}
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
        return cpp::typeerasure::TypeInfo::get<T>() == _typeInfo;
    }

    /**
     * \brief Returns a readonly reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    const std::decay_t<T>& get() const
    {
        SIPLASPLAS_ASSERT_FALSE(empty());
#ifdef SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS
        SIPLASPLAS_ASSERT_TRUE(hasType<std::decay_t<T>>())("SimpleAny has type '{}', requested '{}' instead", typeInfo().typeName(), ctti::type_id<std::decay_t<T>>().name());
#endif
        return *reinterpret_cast<const std::decay_t<T>*>(ConstNonOwningStorage::storage(_typeInfo));
    }

    /**
     * \brief Returns the type information of the hosted type
     */
    cpp::typeerasure::TypeInfo typeInfo() const
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
            _typeInfo = cpp::typeerasure::TypeInfo::get<T>();
            ConstNonOwningStorage::rebind(&value);
        }
        else
        {
            _typeInfo.copyAssign(ConstNonOwningStorage::storage(_typeInfo), &value);
        }

        return *this;
    }

    /**
     * \brief Returns the storage backend of the SimpleAny object
     */
    const ConstNonOwningStorage& getStorage() const
    {
        return *static_cast<const ConstNonOwningStorage*>(this);
    }

private:
    cpp::typeerasure::TypeInfo _typeInfo;
};

/**
 * \brief cpp::SimpleAny specialization for non-owning references to existing objects.
 * See cpp::NonOwningStorage
 */
template<>
class SimpleAny<NonOwningStorage> : public NonOwningStorage
{
public:
    struct EmptyTag {};

    SimpleAny() :
        NonOwningStorage{nullptr},
        _typeInfo{cpp::typeerasure::TypeInfo::get<EmptyTag>()}
    {}

    /**
     * \brief Checks whether the any has an object hosted in or
     * if is empty
     *
     * \returns True if the any is empty (There's no hosted object), false
     * instead
     */
    bool empty() const
    {
        return hasType<EmptyTag>();
    }

    template<typename T>
    SimpleAny(T& value) :
        NonOwningStorage(const_cast<std::decay_t<T>*>(&value)),
        _typeInfo{cpp::typeerasure::TypeInfo::get<std::decay_t<T>>()}
    {}

    SimpleAny(void* object, const cpp::typeerasure::TypeInfo& typeInfo) :
        NonOwningStorage{object},
        _typeInfo{typeInfo}
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
        return cpp::typeerasure::TypeInfo::get<T>() == _typeInfo;
    }

    /**
     * \brief Returns a readonly reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    std::decay_t<T>& get() const
    {
        SIPLASPLAS_ASSERT_FALSE(empty());
#ifdef SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS
        SIPLASPLAS_ASSERT_TRUE(hasType<std::decay_t<T>>())("SimpleAny has type '{}', requested '{}' instead", typeInfo().typeName(), ctti::type_id<std::decay_t<T>>().name());
#endif
        return const_cast<std::decay_t<T>&>(*reinterpret_cast<const std::decay_t<T>*>(NonOwningStorage::storage(_typeInfo)));
    }

    /**
     * \brief Returns a reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     */
    template<typename T>
    std::decay_t<T>& get()
    {
        SIPLASPLAS_ASSERT_FALSE(empty());
#ifdef SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS
        SIPLASPLAS_ASSERT_TRUE(hasType<std::decay_t<T>>())("SimpleAny has type '{}', requested '{}' instead", typeInfo().typeName(), ctti::type_id<std::decay_t<T>>().name());
#endif
        return *reinterpret_cast<std::decay_t<T>*>(NonOwningStorage::storage(_typeInfo));
    }

    /**
     * \brief Returns the type information of the hosted type
     */
    cpp::typeerasure::TypeInfo typeInfo() const
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
    SimpleAny& operator=(T& value)
    {
        if(!hasType<T>())
        {
            _typeInfo = cpp::typeerasure::TypeInfo::get<T>();
            NonOwningStorage::rebind(&value);
        }
        else
        {
            _typeInfo.copyAssign(NonOwningStorage::storage(_typeInfo), &value);
        }

        return *this;
    }

    /**
     * \brief Assigns an rvalue of type T
     *
     * If the current hosted type is T, performs a move assignment of \p value
     * into the hosted object. Else, an exception is thrown (Cannot rebind
     * an lvalue reference represented by the SimpleAny from an rvalue reference)
     *
     * \param value Value to be assigned to the any
     * \returns A reference to `*this`
     */
    template<typename T>
    SimpleAny& operator=(T&& value)
    {
        if(!hasType<std::decay_t<T>>())
        {
            throw cpp::exception<std::runtime_error>(
                "Cannot assign an rvalue of type {} to an lvalue reference of type {}",
                ctti::type_id(value).name(),
                _typeInfo.typeName()
            );
        }
        else
        {
            get<std::decay_t<T>>() = std::move(value);
            return *this;
        }
    }

    /**
     * \brief Returns the storage backend of the SimpleAny object
     */
    const NonOwningStorage& getStorage() const
    {
        return *static_cast<const NonOwningStorage*>(this);
    }

private:
    cpp::typeerasure::TypeInfo _typeInfo;
};

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
     * \brief Type used to represent empty state
     */
    struct EmptyTag {};

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
     * \brief Constructs an empty SimpleAny
     */
    SimpleAny() :
        _typeInfo{cpp::typeerasure::TypeInfo::get<EmptyTag>()}
    {}

    /**
     * \brief Default constructs a value of the given type
     *
     * The behavior is undefined if the given type is not default constructible
     *
     * \param typeInfo Type of the value
     */
    SimpleAny(const cpp::typeerasure::TypeInfo& typeInfo) :
        _typeInfo{typeInfo}
    {
        _typeInfo.defaultConstruct(Storage::storage(_typeInfo));
    }

    /**
     * \brief Checks whether the any has an object hosted in or
     * if is empty
     *
     * \returns True if the any is empty (There's no hosted object), false
     * instead
     */
    bool empty() const
    {
        return hasType<EmptyTag>();
    }

    /**
     * \brief Constructs an any of type T from an lvalue of type T
     *
     * A copy is done from the T lvalue argument to the any storage
     *
     * \param value A value of type T to store in the any
     */
    template<typename T, typename = std::enable_if_t<
        !std::is_base_of<
            SimpleAny,
            std::decay_t<T>
        >::value
    >>
    SimpleAny(const T& value) :
        _typeInfo{cpp::typeerasure::TypeInfo::get<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(Storage::template objectFitsInStorage<T>());
        _typeInfo.copyConstruct(Storage::storage(_typeInfo), &value);
    }

    template<typename OtherStorage>
    SimpleAny(const SimpleAny<OtherStorage>& other) :
        _typeInfo{other.typeInfo()}
    {
        _typeInfo.copyConstruct(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
    }

    template<typename OtherStorage>
    SimpleAny(SimpleAny<OtherStorage>&& other) :
        _typeInfo{other.typeInfo()}
    {
        _typeInfo.moveConstruct(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
    }

    SimpleAny(const SimpleAny& other) :
        _typeInfo{other.typeInfo()}
    {
        _typeInfo.copyConstruct(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
    }

    SimpleAny(SimpleAny&& other) :
        _typeInfo{other.typeInfo()}
    {
        _typeInfo.moveConstruct(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
    }

    /**
     * \brief Checks if the any has a value of type T
     *
     * \returns True if the hosted type is **exactly** T, false otherwise
     */
    template<typename T>
    bool hasType() const
    {
        return cpp::typeerasure::TypeInfo::get<T>() == _typeInfo;
    }

    /**
     * \brief Returns a readonly reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     * \returns A reference to the hosted object it the hosted object is not a pointer.
     * If the hosted object is a pointer, returns a reference to the pointed object
     */
    template<typename T>
    const std::decay_t<T>& get() const
    {
        SIPLASPLAS_ASSERT_FALSE(empty());
#ifdef SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS
        SIPLASPLAS_ASSERT_TRUE(hasType<std::decay_t<T>>())("SimpleAny has type '{}', requested '{}' instead", typeInfo().typeName(), ctti::type_id<std::decay_t<T>>().name());
#endif
        if(_typeInfo.isPointer())
        {
            return **reinterpret_cast<const std::decay_t<T>* const *>(Storage::storage(_typeInfo));
        }
        else
        {
            return *reinterpret_cast<const std::decay_t<T>*>(Storage::storage(_typeInfo));
        }
    }

    /**
     * \brief Returns a reference to the hosted object
     *
     * \tparam T Type of the returned object. If T is different from the hosted object
     * type, the behavior is undefined (See hasType()).
     * \returns A reference to the hosted object it the hosted object is not a pointer.
     * If the hosted object is a pointer, returns a reference to the pointed object
     */
    template<typename T>
    std::decay_t<T>& get()
    {
        SIPLASPLAS_ASSERT_FALSE(empty());
#ifdef SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS
        SIPLASPLAS_ASSERT_TRUE(hasType<std::decay_t<T>>())("SimpleAny has type '{}', requested '{}' instead", typeInfo().typeName(), ctti::type_id<std::decay_t<T>>().name());
#endif
        if(_typeInfo.isPointer())
        {
            return **reinterpret_cast<std::decay_t<T>**>(Storage::storage(_typeInfo));
        }
        else
        {
            return *reinterpret_cast<std::decay_t<T>*>(Storage::storage(_typeInfo));
        }
    }

    /**
     * \brief Returns the type information of the hosted type
     */
    cpp::typeerasure::TypeInfo typeInfo() const
    {
        return _typeInfo;
    }

    /**
     * \brief Returns a reference any to the hosted object
     */
    cpp::SimpleAny<cpp::NonOwningStorage> getReference()
    {
        return {Storage::storage(_typeInfo), _typeInfo};
    }

    /**
     * \brief Returns a const reference any to the hosted object
     */
    cpp::SimpleAny<cpp::ConstNonOwningStorage> getReference() const
    {
        return {Storage::storage(_typeInfo), _typeInfo};
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
            _typeInfo.destroy(Storage::storage(_typeInfo));
            _typeInfo = cpp::typeerasure::TypeInfo::get<T>();
            SIPLASPLAS_ASSERT_TRUE(Storage::template objectFitsInStorage<T>());
            _typeInfo.copyConstruct(Storage::storage(_typeInfo), &value);
        }
        else
        {
            _typeInfo.copyAssign(Storage::storage(_typeInfo), &value);
        }

        return *this;
    }

    SimpleAny& operator=(const SimpleAny& other)
    {
        if(!sameType(*this, other))
        {
            _typeInfo.destroy(Storage::storage(_typeInfo));
            _typeInfo = other._typeInfo;
            _typeInfo.copyConstruct(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
        }
        else
        {
            _typeInfo.copyAssign(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
        }

        return *this;
    }

    SimpleAny& operator=(SimpleAny&& other)
    {
        if(!sameType(*this, other))
        {
            _typeInfo.destroy(Storage::storage(_typeInfo));
            _typeInfo = other._typeInfo;
            _typeInfo.moveConstruct(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
        }
        else
        {
            _typeInfo.moveAssign(Storage::storage(_typeInfo), other.storage(other.typeInfo()));
        }

        return *this;
    }

    ~SimpleAny()
    {
        _typeInfo.destroy(Storage::storage(_typeInfo));
    }

    /**
     * \brief Returns the storage backend of the SimpleAny object
     */
    const Storage& getStorage() const
    {
        return *static_cast<const Storage*>(this);
    }

private:
    cpp::typeerasure::TypeInfo _typeInfo;

    template<typename T, typename... Args>
    SimpleAny(meta::identity<T>, Args&&... args) :
        _typeInfo{cpp::typeerasure::TypeInfo::get<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(Storage::template objectFitsInStorage<T>());
        features::Constructible::apply<T>(Storage::storage(_typeInfo), std::forward<Args>(args)...);
    }
};


/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 8 byte dead pool storage
 */
using SimpleAny8  = SimpleAny<DeadPoolStorage<8>>;

/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 16 byte dead pool storage
 */
using SimpleAny16 = SimpleAny<DeadPoolStorage<16>>;

/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 32 byte dead pool storage
 */
using SimpleAny32 = SimpleAny<DeadPoolStorage<32>>;

/**
 * \ingroup type-erasure
 * \brief A SimpleAny with 64 byte dead pool storage
 */
using SimpleAny64 = SimpleAny<DeadPoolStorage<64>>;

/**
 * \ingroup type-erasure
 * \brief A non-owning SimpleAny that (const) references an existing object
 */
using ConstReferenceSimpleAny = SimpleAny<ConstNonOwningStorage>;

/**
 * \ingroup type-erasure
 * \brief A non-owning SimpleAny that references an existing object
 */
using ReferenceSimpleAny = SimpleAny<NonOwningStorage>;


}

#endif // SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
