
#ifndef SIPLASPLAS_METATYPE_METATYPE_HPP
#define SIPLASPLAS_METATYPE_METATYPE_HPP

#include <unordered_map>
#include <memory>

#include <ctti/type_id.hpp>
#include <sstream>

namespace cpp
{
    class MetaTypeSystem
    {
    public:
        template<typename T>
        static void registerMetatype()
        {
            std::unique_ptr<MetaTypeBase> metaType = std::unique_ptr<MetaType<T>>( new MetaType<T>{} );

            registry[ctti::unnamed_type_id<T>()] = std::move(metaType);
        }

        static void* create(const std::string& typeName)
        {
            // MSVC mangling adds struct/class qualifiers to type names
            std::string className = "class " + typeName;
            std::string structName = "struct " + typeName;

            void* result = nullptr;

            if (!(result = try_create(typeName)))
            {
                if (!(result = try_create(className)))
                {
                    if (!(result = try_create(structName)))
                    {
                        throw std::runtime_error(
                            "[METATYPE_SYSTEM] Type '" + typeName + "' not registered!\n"
                            " - As plain type (" + typeName + "): Hash " + std::to_string(ctti::id_from_name(typeName).hash()) + "\n"
                            " - As class type (" + className + "): Hash " + std::to_string(ctti::id_from_name(className).hash()) + "\n"
                            " - As struct type (" + structName + "): Hash " + std::to_string(ctti::id_from_name(structName).hash()) + "\n"
                            + dump()
                        );
                    }
                }
            }

            return result;
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

        static std::string dump()
        {
            std::ostringstream os;

            os << "[METATYPE_SYSTEM] REGISTRY DUMP:" << std::endl;

            for(const auto& keyValue : registry)
            {
                const auto& id = keyValue.first;

                os << " - Hash: " << id.hash() << std::endl;
            }

            return os.str();
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

        static void* try_create(const std::string& typeName)
        {
            auto id = ctti::id_from_name(typeName);

            auto it = registry.find(id);

            if (it != registry.end())
            {
                return it->second->create();
            }
            else
            {
                return nullptr;
            }
        }
    };

    
    MetaTypeSystem::MetaTypeRegistry MetaTypeSystem::registry;
    template<typename T>
    bool MetaTypeSystem::TypeRegister<T>::registered = false;
}

#endif // SIPLASPLAS_METATYPE_METATYPE_HPP
