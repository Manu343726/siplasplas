
#ifndef SIPLASPLAS_METATYPE_METATYPE_HPP
#define SIPLASPLAS_METATYPE_METATYPE_HPP

#include <unordered_map>
#include <memory>

#include <ctti/type_id.hpp>
#include <iostream>

namespace cpp
{
    class MetaTypeSystem
    {
    public:
        template<typename T>
        static ctti::unnamed_type_id_t registerMetatype()
        {
            std::cout << "Registering type " << ctti::type_id<T>().name() << std::endl;

            std::unique_ptr<MetaTypeBase> metaType = std::make_unique<MetaType<T>>( new MetaType<T>{} );

            registry[MetaType<T>::typeId.hash()] = std::move(metaType);

            return MetaType<T>::typeId; 
        }

        static void* create(const std::string& typeName)
        {
            auto hash = ctti::detail::sid_hash(typeName.size()+1, typeName.data());

            auto it = registry.find(hash);

            if(it != registry.end())
            {
                return registry[hash]->create();
            }
            else
            {
                for(const auto& keyValue : registry)
                {
                    std::cout << "[METATYPE_SYSTEM] REGISTRY DUMP: " << keyValue.first << std::endl;
                }

                throw std::runtime_error("[METATYPE_SYSTEM] Type not registered!");
            }
        }

    private:
        template<typename T>
        struct TypeRegister
        {
            TypeRegister()
            {
                if(!registered)
                {
                    registerMetatype<T>();
                    registered = false;
                }
            }

        private:
            static bool registered;
        };

    public:
        template<typename T>
        struct Register
        {
        protected:
            Register() = default;
        private:
            TypeRegister<T> _register;
        };

    private:
        class MetaTypeBase
        {
        public:
            virtual ~MetaTypeBase() = default;
            virtual void* create() const;
        };

        using MetaTypeRegistry = std::unordered_map<ctti::detail::hash_t, std::unique_ptr<MetaTypeBase>>;
        
        static MetaTypeRegistry registry;

        
        template<typename T>
        struct MetaType : public MetaTypeBase
        {
            static constexpr ctti::unnamed_type_id_t typeId = ctti::unnamed_type_id<T>();
            
            void* create() const override
            {
                return static_cast<void*>(new T{});
            }
        }; 
    };

    
    MetaTypeSystem::MetaTypeRegistry MetaTypeSystem::registry;
    template<typename T>
    bool MetaTypeSystem::TypeRegister<T>::registered = false;
}

#endif // SIPLASPLAS_METATYPE_METATYPE_HPP
