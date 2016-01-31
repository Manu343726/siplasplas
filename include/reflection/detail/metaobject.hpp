#ifndef SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_HPP
#define SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_HPP

#include "reflection/detail/metatype.hpp"

#include <memory>
#include <cassert>

namespace cpp
{
    namespace detail
    {
        class ObjectDeleter
        {
        public:
            ObjectDeleter(const cpp::MetaType& type, bool isReference = false) :
                _type{type},
                _isReference{isReference}
            {}

            void operator()(void* object) const
            {
                if(!_isReference)
                {
                    _type.destroy(object);
                }
            }

        private:
            const cpp::MetaType _type;
            const bool _isReference;
        };
    }

    class MetaObject
    {
    public:
        template<typename T>
        static void registerMetaObject()
        {
            cpp::MetaType::registerMetaType<T>();
        }

        static constexpr bool ConstructReference = true;

        MetaObject(const cpp::MetaType& type) :
            _object{type.create(), detail::ObjectDeleter{type}},
            _type{type}
        {}

        MetaObject(const cpp::MetaType& type, void* fromRaw, bool isReference = false) :
            _object{(isReference ? fromRaw : type.create(fromRaw)), detail::ObjectDeleter{type, isReference}},
            _type{type},
            _isReference{isReference}
        {}

        template<typename T>
        MetaObject(const T& value) :
            _object{cpp::MetaType::get<T>().create(&value), detail::ObjectDeleter{cpp::MetaType::get<T>()}},
            _type{cpp::MetaType::get<T>()}
        {}

        template<typename T>
        MetaObject& operator=(const T& value)
        {
            get<T>() = value;
            return *this;
        }

        template<typename T>
        const T& get() const
        {
            assert(ctti::type_id<T>() == _type.type().type_id());
            return *reinterpret_cast<const T*>(_object.get());
        }

        template<typename T>
        T& get()
        {
            assert(ctti::type_id<T>() == _type.type().type_id());
            return *reinterpret_cast<T*>(_object.get());
        }

        template<typename T>
        operator const T&() const
        {
            return get<T>();
        }

        template<typename T>
        operator T&()
        {
            return get<T>();
        }

        cpp::MetaType type() const
        {
            return _type;
        }

        bool isReference() const
        {
            return _isReference;
        }

        MetaObject& operator=(const MetaObject& other)
        {
            assert(type().type().type_id() == other.type().type().type_id());

            _type.assign(_object.get(), other._object.get());

            return *this;
        }

    private:
        std::shared_ptr<void> _object;
        MetaType _type;
        bool _isReference = false;
    };
}

#endif // SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_HPP
