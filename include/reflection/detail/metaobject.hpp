#ifndef SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_HPP
#define SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_HPP

#include "reflection/detail/metatype.hpp"

#include <memory>
#include <cassert>

namespace cpp
{
    class MetaObject
    {
    public:
        template<typename T>
        static void registerMetaObject()
        {
            cpp::MetaType::registerMetaType<T>();
        }

        MetaObject(const cpp::MetaType& type) :
            _object{type.create(), [type](void* object)
                {
                    type.destroy(object);
                }},
            _type{type}
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
            assert(ctti::unnamed_type_id<T>() == _type.type().type_id());
            return *reinterpret_cast<const T*>(_object.get());
        }

        template<typename T>
        T& get()
        {
            assert(ctti::unnamed_type_id<T>() == _type.type().type_id());
            return *reinterpret_cast<T*>(_object.get());
        }
    private:
        std::shared_ptr<void> _object;
        MetaType _type;
    };
}

#endif // SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_HPP
