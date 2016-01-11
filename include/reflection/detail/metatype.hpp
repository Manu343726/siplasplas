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
            static constexpr ctti::unnamed_type_id_t id = ctti::unnamed_type_id<T>();
        };
        
        template<typename T>
        constexpr ctti::unnamed_type_id_t CustomTypeName<T>::id;
        
#define CPP_REFLECTION_CUSTOM_TYPENAME_FOR(type, name) \
        namespace cpp { namespace detail {             \
        template<>                                     \
        struct CustomTypeName<type> {                  \
            static constexpr ::ctti::unnamed_type_id_t id = ::ctti::id_from_name(name); \
        }; \
        constexpr ::ctti::unnamed_type_id_t CustomTypeName<type>::id; }}
    }
    
    class MetaType
    {
    public:
        void* create() const
        {
            return _lifetimeManager->construct();
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
                _blocks.emplace_back(512 * (sizeof(T) + alignof(T)));
            }

            void* construct() override
            {
                return new(allocate()) T();
            }

            void destroy(void* object) override
            {
                reinterpret_cast<T*>(object)->~T();
                deallocate(object);
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

            std::vector<StorageBlock> _blocks;

            cpp::FreeList freeList()
            {
                return _blocks.back().freeList();
            }

            void* allocate(int tries = 1)
            {
                void* slot = freeList().allocate(sizeof(T), alignof(T));

                if(slot)
                    return slot;
                else
                {
                    if(tries < 10)
                    {
                        // Freelist out of space, allocate new block
                        _blocks.emplace_back(512*(sizeof(T) + alignof(T)));

                        // try again
                        return allocate(tries + 1);
                    }
                    else
                    {
                        cpp::Throw<std::runtime_error>() << "Cannot allocate more storage for meta-instances";
                    }
                }
            }

            void deallocate(void* pointer)
            {
                auto blockIt = std::find_if(std::begin(_blocks), std::end(_blocks),
                [pointer](const StorageBlock& block)
                {
                    return block.freeList().belongs_to_storage(pointer);       
                });

                if(blockIt != std::end(_blocks))
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
                _registry[cpp::detail::CustomTypeName<T>::id] = instance;
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
