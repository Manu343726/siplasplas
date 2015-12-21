
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
        static void registerMetatype()
        {
            std::string name = ctti::type_id<T>().name().c_str();
            auto hash = ctti::detail::sid_hash(name.size(), name.data());

            std::unique_ptr<MetaTypeBase> metaType = std::unique_ptr<MetaType<T>>( new MetaType<T>{} );

            registry[hash] = std::move(metaType);
        }

        template<std::size_t N>
        static void* create(const char (&typeName)[N])
        {
            auto hash = ctti::detail::sid_hash(N-1, typeName);

            auto it = registry.find(hash);

            if(it != registry.end())
            {
                return registry[hash]->create();
            }
            else
            {
                std::cout << "[METATYPE_SYSTEM] Hash " << hash << " not found (Typename: " << typeName << ")" << std::endl;

                for(const auto& keyValue : registry)
                {
                    std::cout << "[METATYPE_SYSTEM] REGISTRY DUMP: " << keyValue.first << std::endl;
                }

                throw std::runtime_error("[METATYPE_SYSTEM] Type not registered!");
            }
        }

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
            virtual ~Register() = default;
        private:
            TypeRegister<T> _register;
        };

    private:
        class MetaTypeBase
        {
        public:
            virtual ~MetaTypeBase() = default;
            virtual void* create() const = 0;
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
