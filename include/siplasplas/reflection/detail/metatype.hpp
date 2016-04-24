#ifndef SIPLASPLAS_REFLECTION_METATYPE_HPP
#define SIPLASPLAS_REFLECTION_METATYPE_HPP

#include "type_info.hpp"
#include <siplasplas/allocator/freelist_allocator.hpp>
#include <siplasplas/utility/throw.hpp>
#include <siplasplas/utility/lexical_cast.hpp>
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/reflection/export.hpp>

#include <unordered_map>
#include <vector>
#include <algorithm>

namespace cpp
{
    namespace detail
    {
        template<typename T>
        struct CustomTypeName
        {
            static constexpr ctti::unnamed_type_id_t id()
            {
                return ctti::unnamed_type_id<T>();
            }
        };

#define CPP_REFLECTION_CUSTOM_TYPENAME_FOR(type, name) \
        namespace cpp { namespace detail {             \
        template<>                                     \
        struct CustomTypeName<type> {                  \
            static constexpr ::ctti::unnamed_type_id_t id() \
            {                                               \
                return ::ctti::id_from_name(name);          \
            }                                               \
        }; }}
    }

#define CPP_REFLECTION_FORCE_TYPENAME(type) CPP_REFLECTION_CUSTOM_TYPENAME_FOR(type, #type)

    class SIPLASPLAS_REFLECTION_EXPORT MetaType
    {
    public:
        MetaType() = default;

        void* create() const
        {
            return _lifetimeManager->construct();
        }

        void* create( const void* object) const
        {
            return _lifetimeManager->construct(object);
        }

        void assign(void* object, const void* other) const
        {
            return _lifetimeManager->assign(object, other);
        }

        void destroy(void* object) const
        {
            return _lifetimeManager->destroy(object);
        }

        std::string toString(void* object) const
        {
            return _lifetimeManager->toString(object);
        }

        void* fromString(const std::string& value) const
        {
            return _lifetimeManager->fromString(value);
        }

        const cpp::TypeInfo& type() const
        {
            return _lifetimeManager->type();
        }

        const cpp::TypeInfo& typeInfo() const
        {
            return type();
        }

        const char* typeName() const
        {
            return typeInfo().name();
        }

        template<typename T>
        static void registerMetaType()
        {
            getLifetimeManager<T>();
        }

        template<typename... Ts>
        static void registerMetaTypes()
        {
            TypeRegistration<meta::list<Ts...>>::apply();
        }

        template<typename T>
        static MetaType get()
        {
            return { getLifetimeManager<T>() };
        }

        static MetaType get(const std::string& typeName)
        {
            auto try_get = [](const std::string typeName) -> std::shared_ptr<MetaTypeLifeTimeManagerBase>
            {
                auto id = ctti::id_from_name(typeName);
                auto it = metaTypes().find(id);

                if(it != std::end(metaTypes()))
                    return it->second;
                else
                    return nullptr;
            };

            std::shared_ptr<MetaTypeLifeTimeManagerBase> manager = nullptr;

            if(!(manager = try_get(typeName)) &&
               !(manager = try_get("class " + typeName)) &&
               !(manager = try_get("struct " + typeName)))
            {
                cpp::Throw<std::runtime_error>() << "Type '" << typeName << "' not registered";
            }
            else
                return { manager };
        }

        friend bool operator==(const MetaType& lhs, const MetaType& rhs)
        {
            return lhs.typeInfo() == rhs.typeInfo();
        }

        friend bool operator!=(const MetaType& lhs, const MetaType& rhs)
        {
            return !(lhs == rhs);
        }

        class SIPLASPLAS_REFLECTION_EXPORT MetaTypeLifeTimeManagerBase
        {
        public:
            virtual ~MetaTypeLifeTimeManagerBase() = default;

            virtual void* construct() = 0;
            virtual void destroy(void* object) = 0;
            virtual void* construct(const void* object) = 0;
            virtual void assign(void* object, const void* other) = 0;
            virtual std::string toString(void* object) = 0;
            virtual void* fromString(const std::string& value) = 0;
            virtual const cpp::TypeInfo& type() const = 0;
        };

    private:
        template<typename Ts, typename = void>
        class TypeRegistration;

        template<typename Head, typename... Tail>
        class TypeRegistration<meta::list<Head, Tail...>, void>
        {
        public:
            static void apply()
            {
                MetaType::registerMetaType<Head>();
                TypeRegistration<meta::list<Tail...>>::apply();
            }
        };

        template<typename... Ts>
        class TypeRegistration<meta::list<meta::list<Ts...>>, void> :
            public TypeRegistration<meta::list<Ts...>>
        {};

        template<typename void_>
        class TypeRegistration<meta::list<>, void_>
        {
        public:
            static void apply()
            {}
        };

        template<typename T>
        class MetaTypeLifeTimeManager : public MetaTypeLifeTimeManagerBase
        {
        public:
            MetaTypeLifeTimeManager() :
                _type{cpp::TypeInfo::get<T>()}
            {
                _objectsArena.emplace_back(512 * (sizeof(T) + alignof(T)));
            }

            void* construct(const void* objectValue) override
            {
                return new(allocate_object()) T(*reinterpret_cast<const T*>(objectValue));
            }

            void* construct() override
            {
                return new(allocate_object()) T();
            }

            void assign(void* object, const void* other) override
            {
               *reinterpret_cast<T*>(object) = *reinterpret_cast<const T*>(other);
            }

            void destroy(void* object) override
            {
                reinterpret_cast<T*>(object)->~T();
                deallocate_object(object);
            }

            std::string toString(void* object) override
            {
                return ToString<T>::apply(*static_cast<T*>(object));
            }

            void* fromString(const std::string& value) override
            {
                auto object = FromString<T>::apply(value);
                return construct(&object); // Copy returned object
            }

            const cpp::TypeInfo& type() const override
            {
                return _type;
            }

        private:
            cpp::TypeInfo _type;

            template<typename U, typename = void>
            class ToString
            {
            public:
                static std::string apply(const U& value)
                {
                    std::ostringstream os;

                    os << "'" << ctti::type_id<U>().name() << "' instance "
                       << "@" << static_cast<const void*>(&value);

                    return os.str();
                }
            };

            template<typename U, typename = void>
            class FromString
            {
            public:
                static U apply(const std::string& value)
                {
                    throw std::runtime_error{
                        "lexical_cast not supported for type i'" + std::string(ctti::type_id<U>().name().c_str()) + 
                        "', you must overload operator>>"
                    };
                }
            };

            template<typename U>
            class ToString<U, meta::void_t<decltype(std::declval<std::ostream&>() << std::declval<const U&>())>>
            {
            public:
                static std::string apply(const U& value)
                {
                    return cpp::lexical_cast(value);
                }
            };

            template<typename U>
            class FromString<U, meta::void_t<decltype(std::declval<std::istream&>() >> std::declval<U&>())>>
            {
            public:
                static U apply(const std::string& value)
                {
                    return cpp::lexical_cast<U>(value);
                }
            };

            class StorageBlock
            {
            public:
                StorageBlock(std::size_t bytes) :
                    _block{ new char[bytes]},
                    _bytes{bytes},
                    _freeList{ begin(), end(), sizeof(T), alignof(T) }
                {}

                void* begin() const
                {
                    return _block.get();
                }

                void* end() const
                {
                    return _block.get() + _bytes;
                }

                std::size_t bytes() const
                {
                    return _bytes;
                }

                cpp::FreeListAllocator freeList() const
                {
                    return _freeList;
                }

            private:
                std::unique_ptr<char[]> _block;
                std::size_t _bytes;
                cpp::FreeListAllocator _freeList;
            };

            std::vector<StorageBlock> _objectsArena;

            void* allocate_object()
            {
                return allocate(_objectsArena, sizeof(T), alignof(T));
            }

            void* allocate(std::vector<StorageBlock>& arena, std::size_t sizeOf, std::size_t alignment, int tries = 1)
            {
                void* slot = arena.back().freeList().allocate(sizeOf, alignment);

                if(slot)
                    return slot;
                else
                {
                    if(tries < 10)
                    {
                        // Freelist out of space, allocate new block
                        arena.emplace_back(512*(sizeOf + alignment));

                        // try again
                        return allocate(arena,sizeOf, alignment, tries + 1);
                    }
                    else
                    {
                        cpp::Throw<std::runtime_error>() << "Cannot allocate more storage for meta-instances";
                    }
                }
            }

            void deallocate_object(void* pointer)
            {
                return deallocate(_objectsArena, pointer);
            }

            void deallocate(std::vector<StorageBlock>& arena, void* pointer)
            {
                auto blockIt = std::find_if(std::begin(arena), std::end(arena),
                [pointer](const StorageBlock& block)
                {
                    return block.freeList().belongs_to_storage(pointer);
                });

                if(blockIt != std::end(arena))
                {
                    blockIt->freeList().deallocate(pointer, sizeof(T));
                }
                else
                    cpp::Throw<std::runtime_error>() << "Cannot deallocate object @" << pointer;
            }
        };

        template<typename T>
        static const std::shared_ptr<MetaTypeLifeTimeManagerBase>& getLifetimeManager()
        {
            static std::shared_ptr<MetaTypeLifeTimeManagerBase> instance{ [&]()
            {
                std::shared_ptr<MetaTypeLifeTimeManagerBase> instance{new MetaTypeLifeTimeManager<T>()};
                metaTypes()[cpp::detail::CustomTypeName<T>::id()] = instance;
                return instance;
            }()};

            return instance;
        }

        MetaType(const std::shared_ptr<MetaTypeLifeTimeManagerBase>& lifetimeManager) :
            _lifetimeManager{lifetimeManager}
        {}

        std::shared_ptr<MetaTypeLifeTimeManagerBase> _lifetimeManager;

        using MetaTypeRegistry = std::unordered_map<ctti::type_index, std::shared_ptr<MetaTypeLifeTimeManagerBase>>;

        static MetaTypeRegistry& metaTypes();
    };
}

#endif // SIPLASPLAS_REFLECTION_METATYPE_HPP
