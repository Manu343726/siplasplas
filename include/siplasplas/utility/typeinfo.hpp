#ifndef SIPLASPLAS_UTILITY_TYPEINFO_HPP
#define SIPLASPLAS_UTILITY_TYPEINFO_HPP

#include "function_traits.hpp"
#include <ctti/type_id.hpp>
#include <bitset>
#include <cstdint>
#include <climits>
#include <type_traits>

namespace cpp
{
class TypeInfo
{
public:
    enum class TypeTraitIndex : std::size_t
    {
        // Primary type categories:
        is_void,
        is_null_pointer,
        is_integral,
        is_floating_point,
        is_array,
        is_enum,
        is_union,
        is_class,
        is_function,
        is_pointer,
        is_lvalue_reference,
        is_rvalue_reference,
        is_member_object_pointer,
        is_member_function_pointer,

        // Composite type categories:
        is_fundamental,
        is_arithmetic,
        is_scalar,
        is_object,
        is_compound,
        is_reference,
        is_member_pointer,

        // Type properties:
        is_const,
        is_volatile,
        is_trivial,
        is_trivially_copyable,
        is_standard_layout,
        is_pod,
        is_literal_type,
        is_empty,
        is_polymorphic,
        is_abstract,
        is_signed,
        is_unsigned,
        is_default_constructible,
        is_copy_constructible,
        is_move_constructible,
        is_copy_assignable,
        is_move_assignable,
        is_destructible,
        is_trivially_copy_constructible,
        is_trivially_move_constructible,
        is_trivially_copy_assignable,
        is_trivially_move_assignable,
        is_trivially_destructible,
        is_nothrow_default_constructible,
        is_nothrow_copy_constructible,
        is_nothrow_move_constructible,
        is_nothrow_copy_assignable,
        is_nothrow_move_assignable,
        is_nothrow_destructible,
        has_virtual_destructor,
        ALWAYS_AT_END_PLEASE__ACTUALLY_NOT_A_TRAIT_INDEX
    };

    using BitsetValue = std::uint64_t;

    static_assert(
            static_cast<std::size_t>(TypeTraitIndex::ALWAYS_AT_END_PLEASE__ACTUALLY_NOT_A_TRAIT_INDEX) <= sizeof(BitsetValue)*CHAR_BIT,
            "BitsetValue has no enough bits to store all type traits"
    );

    using TypeTraits = std::bitset<static_cast<std::size_t>(TypeTraitIndex::ALWAYS_AT_END_PLEASE__ACTUALLY_NOT_A_TRAIT_INDEX)>;

    template<typename T>
    static constexpr TypeInfo get()
    {
#define BITSET_TRAIT(trait) (static_cast<std::uint64_t>(std::trait<T>::value) << static_cast<std::size_t>(TypeTraitIndex::trait))
        return{
            ctti::type_id<T>(),
            sizeof(T),
            alignof(T),
            cpp::function_kind<T>(),
            BitsetValue{
                BITSET_TRAIT(is_void) |
                BITSET_TRAIT(is_null_pointer) |
                BITSET_TRAIT(is_integral) |
                BITSET_TRAIT(is_floating_point) |
                BITSET_TRAIT(is_array) |
                BITSET_TRAIT(is_enum) |
                BITSET_TRAIT(is_union) |
                BITSET_TRAIT(is_class) |
                BITSET_TRAIT(is_function) |
                BITSET_TRAIT(is_pointer) |
                BITSET_TRAIT(is_lvalue_reference) |
                BITSET_TRAIT(is_rvalue_reference) |
                BITSET_TRAIT(is_member_object_pointer) |
                BITSET_TRAIT(is_member_function_pointer) |
                BITSET_TRAIT(is_fundamental) |
                BITSET_TRAIT(is_arithmetic) |
                BITSET_TRAIT(is_scalar) |
                BITSET_TRAIT(is_object) |
                BITSET_TRAIT(is_compound) |
                BITSET_TRAIT(is_reference) |
                BITSET_TRAIT(is_member_pointer) |
                BITSET_TRAIT(is_const) |
                BITSET_TRAIT(is_volatile) |
                BITSET_TRAIT(is_trivial) |
                BITSET_TRAIT(is_trivially_copyable) |
                BITSET_TRAIT(is_standard_layout) |
                BITSET_TRAIT(is_pod) |
                BITSET_TRAIT(is_literal_type) |
                BITSET_TRAIT(is_empty) |
                BITSET_TRAIT(is_polymorphic) |
                BITSET_TRAIT(is_abstract) |
                BITSET_TRAIT(is_signed) |
                BITSET_TRAIT(is_unsigned) |
                BITSET_TRAIT(is_default_constructible) |
                BITSET_TRAIT(is_copy_constructible) |
                BITSET_TRAIT(is_move_constructible) |
                BITSET_TRAIT(is_copy_assignable) |
                BITSET_TRAIT(is_move_assignable) |
                BITSET_TRAIT(is_destructible) |
                BITSET_TRAIT(is_trivially_copy_constructible) |
                BITSET_TRAIT(is_trivially_move_constructible) |
                BITSET_TRAIT(is_trivially_copy_assignable) |
                BITSET_TRAIT(is_trivially_move_assignable) |
                BITSET_TRAIT(is_trivially_destructible) |
                BITSET_TRAIT(is_nothrow_default_constructible) |
                BITSET_TRAIT(is_nothrow_copy_constructible) |
                BITSET_TRAIT(is_nothrow_move_constructible) |
                BITSET_TRAIT(is_nothrow_copy_assignable) |
                BITSET_TRAIT(is_nothrow_move_assignable) |
                BITSET_TRAIT(is_nothrow_destructible) |
                BITSET_TRAIT(has_virtual_destructor)
            }
        };
#undef BITSET_TRAIT
    }

    constexpr bool operator()(const TypeTraitIndex typeTrait) const
    {
        return _typeTraits[static_cast<std::size_t>(typeTrait)];
    }

    constexpr ctti::detail::cstring name() const
    {
        return _typeId.name();
    }

    constexpr ctti::detail::cstring typeName() const
    {
        return name();
    }

    constexpr const ctti::type_id_t& type_id() const
    {
        return _typeId;
    }

    constexpr std::size_t sizeOf() const
    {
        return _sizeOf;
    }

    constexpr std::size_t alignment() const
    {
        return _alignment;
    }

    constexpr cpp::FunctionKind kind() const
    {
        return _kind;
    }

    constexpr bool isPointer() const
    {
        return (*this)(TypeTraitIndex::is_pointer) &&
            kind() != cpp::FunctionKind::FREE_FUNCTION;
    }

    friend constexpr bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
    {
        return lhs.type_id() == rhs.type_id();
    }

    friend constexpr bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
    {
        return !(lhs == rhs);
    }

    struct Null {};

    constexpr TypeInfo() :
        TypeInfo{get<Null>()}
    {}

private:
    ctti::type_id_t _typeId;
    std::size_t _sizeOf;
    std::size_t _alignment;
    cpp::FunctionKind _kind;
    TypeTraits _typeTraits;

    constexpr TypeInfo(const ctti::type_id_t& typeId,
                       const std::size_t sizeOf,
                       const std::size_t alignment,
                       const cpp::FunctionKind kind,
                       const BitsetValue typeTraits) :
        _typeId{ typeId },
        _sizeOf{sizeOf},
        _alignment{ alignment },
        _kind{kind},
        _typeTraits{ typeTraits }
    {}
};

using TypeTrait = TypeInfo::TypeTraitIndex;
}

#endif // SIPLASPLAS_UTILITY_TYPEINFO_HPP
