
#ifndef SIPLASPLAS_METATYPE_METATYPE_HPP
#define SIPLASPLAS_METATYPE_METATYPE_HPP

#include <unordered_map>
#include <memory>

#include <ctti/type_id.hpp>
#include <string>
#include <siplasplas/metatype/export.hpp>

namespace cpp
{
    class SIPLASPLAS_METATYPE_EXPORT MetaTypeSystem
    {
    public:
        template<typename T>
        static void registerMetatype()
        {
            std::unique_ptr<MetaTypeBase> metaType = std::unique_ptr<MetaType<T>>( new MetaType<T>{} );

            registry[ctti::unnamed_type_id<T>()] = std::move(metaType);
        }

        static void* create(const std::string& typeName);

        template<typename T, std::size_t N>
        static T* create(const char (&typeName)[N])
        {
            return static_cast<T*>(create(typeName));
        }

        template<typename T>
        static void destroy(T* ptr)
        {
            delete ptr;
        }

        static std::string dump();

    private:
        class MetaTypeBase
        {
        public:
            virtual ~MetaTypeBase() = default;
            virtual void* create() const = 0;
        };

        using MetaTypeRegistry = std::unordered_map<ctti::type_index, std::unique_ptr<MetaTypeBase>>;
        
        static MetaTypeRegistry registry;

        
        template<typename T>
        struct MetaType : public MetaTypeBase
        {
            void* create() const override
            {
                return static_cast<void*>(new T{});
            }
        };

        static void* try_create(const std::string& typeName);
    };
}

#endif // SIPLASPLAS_METATYPE_METATYPE_HPP
