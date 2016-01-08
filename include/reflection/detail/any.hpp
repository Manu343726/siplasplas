#ifndef SIPLASPLAS_REFLECTION_DETAIL_ANY_HPP
#define SIPLASPLAS_REFLECTION_DETAIL_ANY_HPP

#include "reflection/detail/type_info.hpp"
#include "allocator/allocator_utils.hpp"

#include <cassert>
#include <memory>
#include <type_traits>

namespace cpp
{
	namespace detail
	{
		class Any
		{
		public:
			template<typename T, typename = std::enable_if_t<!std::is_same<std::decay_t<T>, cpp::TypeInfo>::value>>
			Any(T&& value) :
				_valueStorage{ new Storage<std::decay_t<T>>{ std::forward<T>(value) } },
				_typeInfo{ cpp::TypeInfo::get<std::decay_t<T>>() }
			{}

			Any(const cpp::TypeInfo& type) :
				_valueStorage{ new(type) DynamicTypeStorage{type} },
				_typeInfo{ type }
			{}

			template<typename T>
			const T& get() const
			{
				assert(ctti::unnamed_type_id<T>() == _typeInfo.type_id());
				return *reinterpret_cast<const T*>(_valueStorage->get());
			}

			template<typename T>
			T& get()
			{
				assert(ctti::unnamed_type_id<T>() == _typeInfo.type_id());
				return *reinterpret_cast<T*>(_valueStorage->get());
			}

			Any& operator=(Any&&) = default;

			template<typename T>
			Any& operator=(T&& value)
			{
				if (_valueStorage->is_dynamic())
					new(&get<std::decay_t<T>>()) std::decay_t<T>{ std::forward<T>(value) };
				else
					get<std::decay_t<T>>() = std::forward<T>(value);

				return *this;
			}

		private:
			struct StorageBase
			{
				virtual ~StorageBase() = default;
				virtual const void* get() const = 0;
				virtual void* get() = 0;
				virtual bool is_dynamic() const = 0;
			};

			template<typename T>
			struct Storage : public StorageBase
			{
				template<typename... Args>
				Storage(Args&&... args) :
					_value{ std::forward<Args>(args)... }
				{}

				const void* get() const override
				{
					return &_value;
				}

				void* get() override
				{
					return &_value;
				}

				bool is_dynamic() const override
				{
					return false;
				}

			private:
				T _value;
			};

			struct DynamicTypeStorage : public StorageBase
			{
				DynamicTypeStorage(const cpp::TypeInfo& type) :
					_alignment{ type.alignment() }
				{}

				const void* get() const override
				{
					return detail::aligned_ptr(const_cast<void*>(reinterpret_cast<const void*>(&_buffer[0])), _alignment);
				}

				void* get() override
				{
					return detail::aligned_ptr(&_buffer[0], _alignment);
				}

				void* operator new(std::size_t bytes, const cpp::TypeInfo& type)
				{
					return ::operator new(sizeof(DynamicTypeStorage) + type.sizeOf() + type.alignment());
				}

				void operator delete(void* pointer)
				{
					return ::operator delete(pointer);
				}

				bool is_dynamic() const override
				{
					return true;
				}

			private:
				std::size_t _alignment;
				char _buffer[1];
			};



			std::unique_ptr<StorageBase> _valueStorage;
			cpp::TypeInfo _typeInfo;
		};
	}
}

#endif // SIPLASPLAS_REFLECTION_DETAIL_ANY_HPP