#ifndef SIPLASPLAS_TYPEERASURE_TYPEINFO_HPP
#define SIPLASPLAS_TYPEERASURE_TYPEINFO_HPP

#include "features/valuesemantics.hpp"
#include <siplasplas/utility/memory_manip.hpp>
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/function_traits.hpp>

namespace cpp
{

namespace typeerasure
{

namespace detail
{

/**
 * \ingroup type-erasure
 * \brief Represents a value semantics operation. See valueSemanticsOperation()
 */
enum class ValueSemanticsOperation : std::size_t
{
    DEFAULT_CONSTRUCT = 0,
    COPY_CONSTRUCT    = 1,
    MOVE_CONSTRUCT    = 2,
    COPY_ASSIGN       = 3,
    MOVE_ASSIGN       = 4,
    DESTROY           = 5
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
        +[](void* object, const void*) {
            features::DefaultConstructible::apply<T>(object);
        },
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

}

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
    detail::ValueSemantics semantics() const
    {
#if UINTPTR_MAX == UINT64_MAX // See constructor bellow
        return cpp::detail::untagPointer(_semantics);
#else
        return _semantics;
#endif
    }

    /**
     * \brief Returns the size of the type
     */
    std::size_t sizeOf() const
    {
        return _sizeOf;
    }

    /**
     * \brief Returns the name of the type
     */
    const char* typeName() const
    {
        return _typeId.name().c_str();
    }

    /**
     * \brief Checks if the type is a pointer type
     */
    bool isPointer() const
    {
        return _isPointer;
    }

    /**
     * \brief Returns the function implementing the given valuesemantics operation
     * for the type
     */
    detail::ValueSemanticsOperationFunction semantics(detail::ValueSemanticsOperation operation) const
    {
        return semantics()(operation);
    }

    /**
     * \brief Default constructs a value of the type
     * If the passed argument is not of the represented type, the behavior is undefined
     *
     * \param where Address of the object to be constructed
     */
    void defaultConstruct(void* where) const
    {
        semantics(detail::ValueSemanticsOperation::DEFAULT_CONSTRUCT)(where, nullptr);
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
        semantics(detail::ValueSemanticsOperation::COPY_CONSTRUCT)(where, other);
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
        semantics(detail::ValueSemanticsOperation::MOVE_CONSTRUCT)(where, const_cast<const void*>(other));
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
        semantics(detail::ValueSemanticsOperation::COPY_ASSIGN)(where, other);
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
        semantics(detail::ValueSemanticsOperation::MOVE_ASSIGN)(where, const_cast<const void*>(other));
    }

    /**
     * \brief Destroys objects of the type
     * If the passed arguments are not of the represented type, the behavior is undefined
     *
     * \param where Pointer to the object to destroy
     */
    void destroy(void* where) const
    {
        if(!isPointer())
        {
            semantics(detail::ValueSemanticsOperation::DESTROY)(where, nullptr);
        }
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
        _semantics{cpp::detail::tagPointer(&detail::valueSemanticsOperation<T>, alignof(T))},
        _sizeOf{sizeof(T)},
        _typeId{ctti::type_id<T>()},
        _isPointer{
            std::is_pointer<T>::value &&
            cpp::function_kind<T>() != cpp::FunctionKind::FREE_FUNCTION
        }
    {
        static_assert(alignof(T) < (1 << 16), "Alignment of T cannot be tagged in a pointer, its value overflows a 16 bit unsigned integer");
    }

public:
    /**
     * \brief returns the alignment of the type
     */
    std::size_t alignment() const
    {
        return cpp::detail::readTaggedPointer(_semantics);
    }

private:
#else
        _semantics{&valueSemanticsOperation<T>},
        _alignment{alignof(T)},
        _sizeOf{sizeof(T)},
        _typeId{ctti::type_id<T>()},
        _isPointer{
            std::is_pointer<T>::value &&
            cpp::function_kind<T>() != cpp::FunctionKind::FREE_FUNCTION
        }
    {}

public:
    /**
     * \brief returns the alignment of the type
     */
    constexpr std::size_t alignment() const
    {
        return _alignment;
    }

private:
    std::size_t _alignment;
#endif // if not 64 bit
    detail::ValueSemantics _semantics;
    std::size_t _sizeOf;
    ctti::type_id_t _typeId;
    bool _isPointer;
};

}

}

#endif // SIPLASPLAS_TYPEERASURE_TYPEINFO_HPP
