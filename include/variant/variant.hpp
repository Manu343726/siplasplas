#ifndef CEPLUSPLUS_VARIANT_H
#define CEPLUSPLUS_VARIANT_H

#include <type_traits>
#include <ctti/type_id.hpp>

#include "variant_visitor.hpp"

namespace cpp
{
    namespace detail
    {
        template<typename...>
        class Variant;

    }
    template<typename... Ts>
    class Variant
    {
    public:
        using tag_t = ctti::type_id_t;

        const tag_t& tag() const
        {
            return _tag;
        }

        tag_t& tag()
        {
            return _tag;
        }

        template<typename T>
        const T& get() const
        {
            return *storageAs<T>();
        }

        template<typename T>
        T& get()
        {
            return *storageAs<T>();
        }

        Variant() :
            _tag{ctti::type_id<void>()}
        {}

        Variant(const Variant& other) :
            _tag{other.tag()}
        {

        }

        Variant(Variant&& other) :
            _tag{other.tag()},
            _storage{std::move(other._storage)}
        {
        }

        template<typename T, typename = typename std::enable_if<!std::is_same<typename std::decay<T>::type, Variant>::value>::type>
        Variant(T&& value) :
            _tag{ctti::type_id<typename std::decay<T>::type>()}
        {
            new(rawStorage()) typename std::decay<T>::type{std::forward<T>(value)};
        }

        ~Variant()
        {
            VariantExecutor<Ts...>::destroy(*this);
        }

        template<typename T>
        Variant& operator=(T&& value)
        {
            this->~Variant();
            _tag = ctti::type_id<typename std::decay<T>::type>();
            new(rawStorage()) typename std::decay<T>::type{std::forward<T>(value)};
            return *this;
        }

        Variant& operator=(const Variant& other)
        {
            VariantExecutor<Ts...>::copy_assign(*this, other);
            return *this;
        }

        Variant& operator=(Variant&& other)
        {
            VariantExecutor<Ts...>::move_assign(*this, std::move(other));
            return *this;
        }

        template<typename Result, typename... Fs>
        Result visit(Fs... fs)
        {
            return VariantExecutor<Ts...>::visit(*this, ::cpp::visitor<Result>(fs...));
        }

        template<typename Result, typename... Fs>
        Result visit(Fs... fs) const
        {
            return VariantExecutor<Ts...>::const_visit(*this, ::cpp::visitor<Result>(fs...));
        }

        template<typename F>
        typename F::ResultType visit(F f)
        {
            return VariantExecutor<Ts...>::visit(*this, f);
        }

        template<typename F>
        typename F::ResultType visit(F f) const
        {
            return VariantExecutor<Ts...>::const_visit(*this, f);
        }
 
    private:
        using storage_t = typename std::aligned_union<0, Ts...>::type;
        
        storage_t _storage;
        tag_t _tag;

        void* rawStorage()
        {
            return std::addressof(_storage);
        }

        const void* rawStorage() const
        {
            return std::addressof(_storage);
        }

        template<typename T>
        T* storageAs()
        {
            return reinterpret_cast<T*>(rawStorage());
        }

        template<typename T>
        const T* storageAs() const
        {
            return reinterpret_cast<const T*>(rawStorage());
        }


        template<typename...>
        struct typelist {};

        template<typename>
        struct VariantExecutorImpl;

        template<typename... Args>
        using VariantExecutor = VariantExecutorImpl<typelist<Args...>>;

        template<typename Head, typename... Tail>
        using NextVariantExecutor = VariantExecutorImpl<typelist<Tail...>>;

        template<typename Head>
        struct VariantExecutorImpl<typelist<Head>>
        {
            template<typename... Args>
            static bool construct(Variant& v, Args&&... args)
            {
                if(v.tag() == ctti::type_id<Head>())
                {
                    new (v.rawStorage()) Head(std::forward<Args>(args)...);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            static bool destroy(Variant& v)
            {
                if(v.tag() == ctti::type_id<Head>())
                {
                    v.storageAs<Head>()->~Head();
                    return true;
                }
                else
                {
                    return false;
                }
            }

            static bool move(Variant& v, Variant&& other)
            {
                if(other.tag() == ctti::type_id<Head>())
                {   
                    v._tag = std::move(other.tag());
                    other._tag = ctti::type_id<void>();

                    return VariantExecutor<Ts...>::construct(v, std::move(other.get<Head>()));
                }
                else
                {
                    return false;
                }
            }

            static bool copy(Variant& v, const Variant& other)
            {
                if(other.tag() == ctti::type_id<Head>())
                {
                    v.tag() = other.tag();
                    return VariantExecutor<Ts...>::construct(v, other.get<Head>());
                }
                else
                {
                    return false;
                }
            }

            static bool move_assign(Variant& v, Variant&& other)
            {
                if(other.tag() == ctti::type_id<Head>())
                {
                    if(v.tag() == other.tag())
                    {
                        v.get<Head>() = std::move(other.get<Head>());
                        return true;
                    }
                    else
                    {
                        if(VariantExecutor<Ts...>::destroy(v))
                        {
                            v.tag() = other.tag();
                            return VariantExecutor<Ts...>::move(v, std::move(other));
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }

            static bool copy_assign(Variant& v, const Variant& other)
            {
                if(other.tag() == ctti::type_id<Head>())
                {
                    if(v.tag() == other.tag())
                    {
                        v.get<Head>() = other.get<Head>();
                        return true;
                    }
                    else
                    {   
                        if(VariantExecutor<Ts...>::destroy(v))
                        {
                            v.tag() = other.tag();
                            return VariantExecutor<Ts...>::copy(v, other);
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }

            template<typename F>
            static typename F::ResultType const_visit(const Variant& v, F f)
            {
                if(v.tag() == ctti::type_id<Head>())
                {
                    return f(v.get<Head>());
                }
                else
                {
                    throw std::runtime_error{"Cannot visit variant{" + std::string{v.tag().name().c_str()} + "}"};
                }
            }
            
            template<typename F>
            static typename F::ResultType visit(Variant& v, F f)
            {
                if(v.tag() == ctti::type_id<Head>())
                {
                    return f(v.get<Head>());
                }
                else
                {
                    throw std::runtime_error{"Cannot visit variant{" + std::string{v.tag().name().c_str()} + "}"};
                }
            }
        };

        template<typename Head, typename Second, typename... Tail>
        struct VariantExecutorImpl<typelist<Head, Second, Tail...>>
        {
            template<typename... Args>
            static bool construct(Variant& v, Args&&... args)
            {
                return VariantExecutor<Head>::construct(v, std::forward<Args>(args)...) ||
                       NextVariantExecutor<Head, Second, Tail...>::construct(v, std::forward<Args>(args)...);
            }

            static bool destroy(Variant& v)
            {
                return VariantExecutor<Head>::destroy(v) || NextVariantExecutor<Head, Second, Tail...>::destroy(v);
            }

            static bool move(Variant v, Variant&& other)
            {
                return VariantExecutor<Head>::move(v, std::move(other)) || 
                       NextVariantExecutor<Head, Second, Tail...>::move(v, std::move(other));
            }

            static bool copy(Variant& v, const Variant& other)
            {
                return VariantExecutor<Head>::copy(v, other) ||
                       NextVariantExecutor<Head, Second, Tail...>::copy(v, other);
            }

            static bool move_assign(Variant& v, Variant&& other)
            {
                return VariantExecutor<Head>::move_assign(v, std::move(other)) ||
                       NextVariantExecutor<Head, Second, Tail...>::move_assign(v, std::move(other));
            }

            static bool copy_assignment(Variant& v, const Variant& other)
            {
                return VariantExecutor<Head>::copy_assign(v, other) ||
                       NextVariantExecutor<Head, Second, Tail...>::copy_assign(v, other);
            }

            template<typename F>
            static typename F::ResultType const_visit(const Variant& v, F f)
            {
                if(v.tag() == ctti::type_id<Head>())
                {
                    return f(v.get<Head>());
                }
                else
                {
                    return NextVariantExecutor<Head, Second, Tail...>::const_visit(v, f);
                }
            }
            
            template<typename F>
            static typename F::ResultType visit(Variant& v, F f)
            {
                if(v.tag() == ctti::type_id<Head>())
                {
                    return f(v.get<Head>());
                }
                else
                {
                    return NextVariantExecutor<Head, Second, Tail...>::visit(v, f);
                }
            }
        };
    };
}

#endif // CEPLUSPLUS_VARIANT_H
