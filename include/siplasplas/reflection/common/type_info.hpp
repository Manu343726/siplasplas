#ifndef SIPLASPLAS_REFLECTION_TYPE_INFO_HPP
#define SIPLASPLAS_REFLECTION_TYPE_INFO_HPP

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
			is_trivially_default_constructible,
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
			return{
				ctti::type_id<T>(),
				sizeof(T),
				alignof(T),
				BitsetValue{
					// Primary type categories:
					(static_cast<std::size_t>(TypeTraitIndex::is_void)                    << std::is_void<T>::value)                    |
					(static_cast<std::size_t>(TypeTraitIndex::is_null_pointer)            << std::is_null_pointer<T>::value)            |
					(static_cast<std::size_t>(TypeTraitIndex::is_integral)                << std::is_integral<T>::value)                |
					(static_cast<std::size_t>(TypeTraitIndex::is_floating_point)          << std::is_floating_point<T>::value)          |
					(static_cast<std::size_t>(TypeTraitIndex::is_array)                   << std::is_array<T>::value)                   |
					(static_cast<std::size_t>(TypeTraitIndex::is_enum)                    << std::is_enum<T>::value)                    |
					(static_cast<std::size_t>(TypeTraitIndex::is_union)                   << std::is_union<T>::value)                   |
					(static_cast<std::size_t>(TypeTraitIndex::is_class)                   << std::is_class<T>::value)                   |
					(static_cast<std::size_t>(TypeTraitIndex::is_function)                << std::is_function<T>::value)                |
					(static_cast<std::size_t>(TypeTraitIndex::is_pointer)                 << std::is_pointer<T>::value)                 |
					(static_cast<std::size_t>(TypeTraitIndex::is_lvalue_reference)        << std::is_lvalue_reference<T>::value)        |
					(static_cast<std::size_t>(TypeTraitIndex::is_rvalue_reference)        << std::is_rvalue_reference<T>::value)        |
					(static_cast<std::size_t>(TypeTraitIndex::is_member_object_pointer)   << std::is_member_object_pointer<T>::value)   |
					(static_cast<std::size_t>(TypeTraitIndex::is_member_function_pointer) << std::is_member_function_pointer<T>::value) |

					// Composite type categories:
					(static_cast<std::size_t>(TypeTraitIndex::is_fundamental)    << std::is_fundamental<T>::value)    |
					(static_cast<std::size_t>(TypeTraitIndex::is_arithmetic)     << std::is_arithmetic<T>::value)     |
					(static_cast<std::size_t>(TypeTraitIndex::is_scalar)         << std::is_scalar<T>::value)         |
					(static_cast<std::size_t>(TypeTraitIndex::is_object)         << std::is_object<T>::value)         |
					(static_cast<std::size_t>(TypeTraitIndex::is_compound)       << std::is_compound<T>::value)       |
					(static_cast<std::size_t>(TypeTraitIndex::is_reference)      << std::is_reference<T>::value)      |
					(static_cast<std::size_t>(TypeTraitIndex::is_member_pointer) << std::is_member_pointer<T>::value) |

					// Type properties:
					(static_cast<std::size_t>(TypeTraitIndex::is_const)                           << std::is_const<T>::value)                           |
					(static_cast<std::size_t>(TypeTraitIndex::is_volatile)                        << std::is_volatile<T>::value)                        |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivial)                         << std::is_trivial<T>::value)                         |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivially_copyable)              << std::is_trivially_copyable<T>::value)              |
					(static_cast<std::size_t>(TypeTraitIndex::is_standard_layout)                 << std::is_standard_layout<T>::value)                 |
					(static_cast<std::size_t>(TypeTraitIndex::is_pod)                             << std::is_pod<T>::value)                             |
					(static_cast<std::size_t>(TypeTraitIndex::is_literal_type)                    << std::is_literal_type<T>::value)                    |
					(static_cast<std::size_t>(TypeTraitIndex::is_empty)                           << std::is_empty<T>::value)                           |
					(static_cast<std::size_t>(TypeTraitIndex::is_polymorphic)                     << std::is_polymorphic<T>::value)                     |
					(static_cast<std::size_t>(TypeTraitIndex::is_abstract)                        << std::is_abstract<T>::value)                        |
					(static_cast<std::size_t>(TypeTraitIndex::is_signed)                          << std::is_signed<T>::value)                          |
					(static_cast<std::size_t>(TypeTraitIndex::is_unsigned)                        << std::is_unsigned<T>::value)                        |
					(static_cast<std::size_t>(TypeTraitIndex::is_default_constructible)           << std::is_default_constructible<T>::value)           |
					(static_cast<std::size_t>(TypeTraitIndex::is_copy_constructible)              << std::is_copy_constructible<T>::value)              |
					(static_cast<std::size_t>(TypeTraitIndex::is_move_constructible)              << std::is_move_constructible<T>::value)              |
					(static_cast<std::size_t>(TypeTraitIndex::is_copy_assignable)                 << std::is_copy_assignable<T>::value)                 |
					(static_cast<std::size_t>(TypeTraitIndex::is_move_assignable)                 << std::is_move_assignable<T>::value)                 |
					(static_cast<std::size_t>(TypeTraitIndex::is_destructible)                    << std::is_destructible<T>::value)                    |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivially_default_constructible) << std::is_trivially_default_constructible<T>::value) |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivially_copy_constructible)    << std::is_trivially_copy_constructible<T>::value)    |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivially_move_constructible)    << std::is_trivially_move_constructible<T>::value)    |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivially_copy_assignable)       << std::is_trivially_copy_assignable<T>::value)       |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivially_move_assignable)       << std::is_trivially_move_assignable<T>::value)       |
					(static_cast<std::size_t>(TypeTraitIndex::is_trivially_destructible)          << std::is_trivially_destructible<T>::value)          |
					(static_cast<std::size_t>(TypeTraitIndex::is_nothrow_default_constructible)   << std::is_nothrow_default_constructible<T>::value)   |
					(static_cast<std::size_t>(TypeTraitIndex::is_nothrow_copy_constructible)      << std::is_nothrow_copy_constructible<T>::value)      |
					(static_cast<std::size_t>(TypeTraitIndex::is_nothrow_move_constructible)      << std::is_nothrow_move_constructible<T>::value)      |
					(static_cast<std::size_t>(TypeTraitIndex::is_nothrow_copy_assignable)         << std::is_nothrow_copy_assignable<T>::value)         |
					(static_cast<std::size_t>(TypeTraitIndex::is_nothrow_move_assignable)         << std::is_nothrow_move_assignable<T>::value)         |
					(static_cast<std::size_t>(TypeTraitIndex::is_nothrow_destructible)            << std::is_nothrow_destructible<T>::value)            |
					(static_cast<std::size_t>(TypeTraitIndex::has_virtual_destructor)             << std::has_virtual_destructor<T>::value)
				}
			};
		}

		constexpr bool operator()(const TypeTraitIndex typeTrait) const
		{
			return _typeTraits[static_cast<std::size_t>(typeTrait)];
		}

		constexpr const char* name() const
		{
			return _typeId.name().c_str();
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

                friend constexpr bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
                {
                    return lhs.type_id() == rhs.type_id();
                }

                friend constexpr bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
                {
                    return !(lhs == rhs);
                }

	private:
		const ctti::type_id_t _typeId;
		const std::size_t _sizeOf;
		const std::size_t _alignment;
		const TypeTraits _typeTraits;

		constexpr TypeInfo(const ctti::type_id_t& typeId,
						   const std::size_t sizeOf,
						   const std::size_t alignment,
						   const BitsetValue typeTraits) :
			_typeId{ typeId },
			_sizeOf{sizeOf},
			_alignment{ alignment },
			_typeTraits{ typeTraits }
		{}
	};

	using TypeTrait = TypeInfo::TypeTraitIndex;
}

#endif // SIPLASPLAS_REFLECTION_TYPE_INFO_HPP
