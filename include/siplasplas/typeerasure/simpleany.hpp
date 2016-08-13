#ifndef SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
#define SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP

#include "features/valuesemantics.hpp"
#include "logger.hpp"
#include <siplasplas/utility/destroy.hpp>
#include <siplasplas/utility/memory_manip.hpp>
#include <siplasplas/utility/assert.hpp>
#include <cstdint>

namespace cpp
{

namespace detail
{

/**
 * \ingroup type-erasure
 * \brief Represents a move semantics operation. See valueSemanticsOperation()
 */
enum class ValueSemanticsOperation : std::size_t
{
    COPY_CONSTRUCT = 0,
    MOVE_CONSTRUCT = 1,
    COPY_ASSIGN    = 2,
    MOVE_ASSIGN    = 3,
    DESTROY        = 4
};

using ValueSemanticsOperationFunction = void(*)(void*, const void*);

/**
 * \ingroup type-erasure
 * \brief Implements a type-erased interface for the value semantics features
 * of a type T
 *
 * The function returns a function pointer with a type erased signature that
 * gives access to the specified value semantics feature. See ValueSemanticsOperation.
 *
 * ``` cpp
 * int i = 42;
 * int* j = malloc(sizeof(i));
 * void (*copyConstructInts)(void*, const void*) = valueSemanticsOperation<int>(ValueSemanticsOperation::COPY_CONSTRUCT);
 *
 * copyConstructInts(j, &i); // copy i into j
 * ```
 *
 * \param operation The operation needed
 *
 * \returns A `void(*)(void*, const void*)` function pointer that implements
 * the required operation
 */
template<typename T>
ValueSemanticsOperationFunction valueSemanticsOperation(ValueSemanticsOperation operation)
{
    static ValueSemanticsOperationFunction operations[] = {
        +[](void* object, const void* other) {
            features::CopyConstructible::apply<T>(object, other);
        },
        +[](void* object, const void* other) {
            features::MoveConstructible::apply<T>(object, const_cast<void*>(other));
        },
        +[](void* object, const void* other) {
            features::CopyAssignable::apply<T>(object, other);
        },
        +[](void* object, const void* other) {
            features::MoveAssignable::apply<T>(object, const_cast<void*>(other));
        },
        +[](void* object, const void*) {
            features::Destructible::apply<T>(object);
        }
    };

    return operations[static_cast<std::size_t>(operation)];
}

using ValueSemantics = decltype(&valueSemanticsOperation<int>);

/**
 * \ingroup type-erasure
 * \brief Contains minimal information to execute the value semantics operations
 * of a type
 *
 * This class stores the alignment and value semantics operations of a type. The value semantics
 * operations are aset of type-erased functions mapping to the different value semantics features
 * (CopyConstructible, DefaultConstructible, etc). This mapping is implemented as a lookup table
 * of function pointers.
 *
 * ``` cpp
 * std::aligned_storage<sizeof(std::string), alignof(std::string)> stringStorage;
 * std::string str = "hello, world!";
 * auto typeInfo = TypeInfo::get<std::string>();
 *
 * // Copy construct the string from str:
 * typeInfo.copyConstruct(&stringStorage, &str);
 *
 * // Or if there's no access to a properly aligned storage, use TypeInfo::alignment():
 * char buffer[64];
 * typeInfo.copyConstruct(
 *     cpp::detail::aligned_ptr(&buffer[0], typeInfo.alignment()), // Get an aligned address first
 *     &str
 *  );
 * ```
 */
class TypeInfo
{
public:
    /**
     * \brief Retuns the type-erased semantics of the type. See valueSemantics().
     */
    ValueSemantics semantics() const
    {
#if UINTPTR_MAX == UINT64_MAX // See constructor bellow
        return cpp::detail::untagPointer(_semantics);
#else
        return _semantics;
#endif
    }

    /**
     * \brief Returns the function implementing the given valuesemantics operation
     * for the type
     */
    ValueSemanticsOperationFunction semantics(ValueSemanticsOperation operation) const
    {
        return semantics()(operation);
    }

    /**
     * \brief Copy constructs values of the type
     * If the passed arguments are not of the represented type, the behavior is undefined
     *
     * \param where Address of the object to be constructed
     * \param other Address of the object to copy from
     */
    void copyConstruct(void* where, const void* other) const
    {
        semantics(ValueSemanticsOperation::COPY_CONSTRUCT)(where, other);
    }

    /**
     * \brief Move constructs values of the type
     * If the passed arguments are not of the represented type, the behavior is undefined
     *
     * \param where Address of the object to be constructed
     * \param other Reference of the object to move from (The pointer is a pointer to the
     * rvalue reference up in the call stack).
     */
    void moveConstruct(void* where, void* other) const
    {
        semantics(ValueSemanticsOperation::MOVE_CONSTRUCT)(where, const_cast<const void*>(other));
    }

    /**
     * \brief Move assigns values of the type
     * If the passed arguments are not of the represented type, the behavior is undefined
     *
     * \param where Address of the object to be assigned to
     * \param other Address of the object to assign from
     */
    void copyAssign(void* where, const void* other) const
    {
        semantics(ValueSemanticsOperation::COPY_ASSIGN)(where, other);
    }

    /**
     * \brief Move assigns values of the type
     * If the passed arguments are not of the represented type, the behavior is undefined
     *
     * \param where Address of the object to be assigned to
     * \param other Reference of the object to assign from (The pointer is a pointer to the
     * rvalue reference up in the call stack).
     */
    void moveAssign(void* where, void* other) const
    {
        semantics(ValueSemanticsOperation::MOVE_ASSIGN)(where, const_cast<const void*>(other));
    }

    /**
     * \brief Destroys objects of the type
     * If the passed arguments are not of the represented type, the behavior is undefined
     *
     * \param where Pointer to the object to destroy
     */
    void destroy(void* where) const
    {
        semantics(ValueSemanticsOperation::DESTROY)(where, nullptr);
    }

    /**
     * \brief Returns the type information of type T
     */
    template<typename T>
    static constexpr TypeInfo get()
    {
        return TypeInfo{meta::identity<T>()};
    }

    friend constexpr bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
    {
        return lhs._semantics == rhs._semantics;
    }

    friend constexpr bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
    {
        return !(lhs == rhs);
    }

private:
    template<typename T>
    constexpr TypeInfo(meta::identity<T>) :
// If virtual addresses have 64 bits, use a use a tagged pointer to store the alignment,
// else use an extra member
#if UINTPTR_MAX == UINT64_MAX
        _semantics{cpp::detail::tagPointer(&valueSemanticsOperation<T>, alignof(T))}
    {
        static_assert(alignof(T) < (1 << 16), "Alignment of T cannot be tagged in a pointer, its value overflows a 16 bit unsigned integer");
    }

public:
    std::size_t alignment() const
    {
        return cpp::detail::readTaggedPointer(_semantics);
    }

private:
#else
        _semantics{&valueSemanticsOperation<T>}
        _alignment{alignof(T)}
    {}

public:
    constexpr std::size_t alignment() const
    {
        return _alignment;
    }

private:
    std::size_t _alignment;
#endif // if not 64 bit
    ValueSemantics _semantics;
};

}

/**
 * \ingroup type-erasure
 * \brief Implements a non-allocating type-erased value container
 *
 * The template BasicSimpleAny implements a type-erased value container, that is,
 * a type that can hold values of different types at runtime. BasicSimpleAny uses
 * a fixed-size buffer as storage to avoid dynamic allocations. The maximum capacity
 * of this storage can be controlled with the \p Capacity template parameter.
 *
 * BasicSimpleAny assumes that types hosted on it satisfy the CopyConstructible,
 * MoveConstructible, CopyAssignable, MoveAssignable, and Destructible. If any of those
 * concepts are not satisfied, an exception would be thrown if the corresponding feature is used.
 * This means you can use non-satisfying types as long as the operation is not needed (Invoked). For example:
 *
 * ``` cpp
 * cpp::BasicSimpleAny<64> any{NoCopyAssignable()}: // Ok, NoCopyAssignable is move constructible
 * auto any2 = cpp::BasicSimpleAny<64>::create<NoCopyAssignable>(); // Ok, NoCopyAssignable is default constructible
 * any2 = any; // EXCEPTION THROWN: NoCopyAssignable is not copy assignable (Noooo, really?)
 * any2 = std::move(any); // Ok, NoCopyAssignable is move assignable
 * ```
 *
 * Note this behavior is undefined and may be disabled in release builds. In general,
 * **using semantics not supporeted by the hosted type has undefined behavior**.
 *
 * \tparam Capacity Size in bytes of the internal storage. **Is not the max type sizeof()**,
 * since some storage may be unused to fit with the type alignment
 */
template<std::size_t Capacity>
class BasicSimpleAny
{
public:
    static constexpr std::size_t STORAGE_CAPACITY = Capacity;

    /**
     * \brief Construct a BasicSimpleAny with an in-place constructed value of
     * type T
     *
     * The value is constructed directly on the any storage, no extra copy operations are done.
     * Arguments are passed as-is to the object constructor.
     *
     * \tparam T Type of the value to be constructed.
     * \param args Constructor arguments
     *
     * \returns A BasicSimpleAny instance hosting an object of type T
     */
    template<typename T, typename... Args>
    static BasicSimpleAny create(Args&&... args)
    {
        return BasicSimpleAny{meta::identity<T>(), std::forward<Args>(args)...};
    }

    /**
     * \brief Constructs an any of type T from an lvalue of type T
     *
     * A copy is done from the T lvalue argument to the any storage
     *
     * \param value A value of type T to store in the any
     */
    template<typename T>
    BasicSimpleAny(const T& value) :
        _typeInfo{detail::TypeInfo::get<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
        _typeInfo.copyConstruct(storage(), &value);
    }

    BasicSimpleAny(const BasicSimpleAny& other) :
        _typeInfo{other._typeInfo}
    {
        _typeInfo.copyConstruct(storage(), other.storage());
    }

    BasicSimpleAny(BasicSimpleAny&& other) :
        _typeInfo{other._typeInfo}
    {
        _typeInfo.moveConstruct(storage(), other.storage());
    }

    /**
     * \brief Checks if the any has a value of type T
     *
     * \returns True if the hosted type is **exactly** T, false otherwise
     */
    template<typename T>
    bool hasType() const
    {
        return detail::TypeInfo::get<T>() == _typeInfo;
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
        return *reinterpret_cast<const T*>(storage());
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
        return *reinterpret_cast<T*>(storage());
    }

    /**
     * \brief Checks if two BasicSimpleAny objects host values of the same type
     *
     * \returns True if both hosted types are **exactly equal** i
     * (i.e. `std::is_same<hosted type of lhs, hosted type of rhs>::value` would yield true).
     */
    template<std::size_t LhsCapacity, std::size_t RhsCapacity>
    static bool sameType(const BasicSimpleAny<LhsCapacity>& lhs, const BasicSimpleAny<RhsCapacity>& rhs)
    {
        return lhs._typeInfo == rhs._typeInfo;
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
    BasicSimpleAny& operator=(const T& value)
    {
        if(!hasType<T>())
        {
            _typeInfo.destroy(storage());
            _typeInfo = detail::TypeInfo::get<T>();
            SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
            _typeInfo.copyConstruct(storage(), &value);
        }
        else
        {
            _typeInfo.copyAssign(storage(), &value);
        }

        return *this;
    }

    BasicSimpleAny& operator=(const BasicSimpleAny& other)
    {
        if(!sameType(*this, other))
        {
            _typeInfo.destroy(storage());
            _typeInfo = other._typeInfo;
            _typeInfo.copyConstruct(storage(), other.storage());
        }
        else
        {
            _typeInfo.copyAssign(storage(), other.storage());
        }

        return *this;
    }

    BasicSimpleAny& operator=(BasicSimpleAny&& other)
    {
        if(!sameType(*this, other))
        {
            _typeInfo.destroy(storage());
            _typeInfo = other._typeInfo;
            _typeInfo.moveConstruct(storage(), other.storage());
        }
        else
        {
            _typeInfo.moveAssign(storage(), other.storage());
        }

        return *this;
    }

    ~BasicSimpleAny()
    {
        _typeInfo.destroy(storage());
    }

private:
    detail::TypeInfo _typeInfo;
    std::array<char, STORAGE_CAPACITY> _storage;

    template<typename T, typename... Args>
    BasicSimpleAny(meta::identity<T>, Args&&... args) :
        _typeInfo{detail::TypeInfo::get<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
        features::Constructible::apply<T>(storage(), std::forward<Args>(args)...);
    }

    void* storage()
    {
        return cpp::detail::aligned_ptr(&_storage[0], _typeInfo.alignment());
    }

    const void* storage() const
    {
        return cpp::detail::aligned_ptr(&_storage[0], _typeInfo.alignment());
    }

    template<typename T>
    bool objectFitsInStorage() const
    {
        return reinterpret_cast<const char*>(storage()) >= _storage.begin() &&
               reinterpret_cast<const char*>(storage()) + sizeof(T) <= _storage.end();
    }
};

using SimpleAny = BasicSimpleAny<64-sizeof(detail::TypeInfo)>;

}

#endif // SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
