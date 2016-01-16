#ifndef SIPLASPLAS_REFLECTION_METATYPE_HPP
#define SIPLASPLAS_REFLECTION_METATYPE_HPP

#include "reflection/detail/type_info.hpp"
#include "allocator/free_list.hpp"
#include "utility/throw.hpp"

#include <unordered_map>

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
    
    class MetaType
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

        const cpp::TypeInfo& type() const
        {
            return _lifetimeManager->type();
        }

        template<typename T>
        static void registerMetaType()
        {
            getLifetimeManager<T>();
        }

        template<typename T>
        static MetaType get()
        {
            return { getLifetimeManager<T>() };
        }

        static MetaType get(const std::string& typeName)
        {
            auto try_get = [](const std::string typeName) -> MetaTypeLifeTimeManagerBase*
            {
                auto id = ctti::id_from_name(typeName);
                auto it = _registry.find(id);

                if(it != std::end(_registry))
                    return it->second;
                else
                    return nullptr;
            };

            MetaTypeLifeTimeManagerBase* manager = nullptr;

            if(!(manager = try_get(typeName)) && 
               !(manager = try_get("class " + typeName)) &&
               !(manager = try_get("struct " + typeName)))
            {
                cpp::Throw<std::runtime_error>() << "Type '" << typeName << "' not registered";
            }
            else
                return { manager };
        }

        class MetaTypeLifeTimeManagerBase
        {
        public:
            virtual ~MetaTypeLifeTimeManagerBase() = default;

            virtual void* construct() = 0;
            virtual void destroy(void* object) = 0;
            virtual void* construct(const void* object) = 0;
            virtual void assign(void* object, const void* other) = 0;
            virtual const cpp::TypeInfo& type() const = 0;
        };

    private:
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

            void assign(void* object, const void* other)
            {
               *reinterpret_cast<T*>(object) = *reinterpret_cast<const T*>(other);
            }

            void destroy(void* object) override
            {
                reinterpret_cast<T*>(object)->~T();
                deallocate_object(object);
            }

            const cpp::TypeInfo& type() const override
            {
                return _type;
            }

        private:
            cpp::TypeInfo _type;

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

                cpp::FreeList freeList() const
                {
                    return _freeList;
                }

            private:
                std::unique_ptr<char[]> _block;
                std::size_t _bytes;
                cpp::FreeList _freeList;
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
        static MetaTypeLifeTimeManagerBase* getLifetimeManager()
        {
            static std::unique_ptr<MetaTypeLifeTimeManagerBase> instance{ [&]()
            {
                MetaTypeLifeTimeManagerBase* instance = new MetaTypeLifeTimeManager<T>();
                _registry[cpp::detail::CustomTypeName<T>::id()] = instance;
                return instance;
            }()};

            return instance.get();
        }

        MetaType(MetaTypeLifeTimeManagerBase* lifetimeManager) :
            _lifetimeManager{lifetimeManager}
        {}

        MetaTypeLifeTimeManagerBase* _lifetimeManager;

        using MetaTypeRegistry = std::unordered_map<ctti::type_index, MetaTypeLifeTimeManagerBase*>;

        static MetaTypeRegistry _registry;
    };

    MetaType::MetaTypeRegistry MetaType::_registry;
}

#endif // SIPLASPLAS_REFLECTION_METATYPE_HPP
