#ifndef CEPLUSPLUS_VARIANT_H
#define CEPLUSPLUS_VARIANT_H

#include <type_traits>
#include <ctti/type_id.hpp>
#include <iostream>
#include <sstream>

#include "variant/visitor.hpp"

namespace cpp
{
    template<typename... Ts>
    class Variant
    {
    private:
        std::string print_variant(const ctti::unnamed_type_id_t& id) const
        {
            std::ostringstream os;
            
            if(id != ctti::unnamed_type_id<void>())
                os << "Variant{" << id.hash() << "}";
            else
                os << "Variant{[Empty]}";

            return os.str();
        }
        void Throw(const std::string& operation, const ctti::unnamed_type_id_t& tag)
        {   
            throw std::runtime_error{print_variant(this->tag()) + 
                ": Cannot " + operation + " " + print_variant(tag)};
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
            template<typename T>
            struct has_type_static : public std::is_same<T, Head>
            {};
            
            static constexpr bool has_type(ctti::unnamed_type_id_t id)
            {
                return id == ctti::unnamed_type_id<Head>();
            }
            
            template<typename... Args>
            static bool construct(Variant& v, Args&&... args)
            {
                if(v.tag() == ctti::unnamed_type_id<Head>())
                {
                    new (v.rawStorage()) Head(std::forward<Args>(args)...);
                    return !v.empty();
                }
                else
                {
                    return false;
                }
            }

            static bool destroy(Variant& v)
            {
                // Take care of empty variants, destroying them is fine
                // (Actually a nop)
                if(v.empty())
                    return true;

                if(v.tag() == ctti::unnamed_type_id<Head>())
                {
                    destructor_call<Head>::apply(v.storageAs<Head>());
                    return true;
                }
                else
                {
                    return false;
                }
            }

            template<typename T, bool isClass = std::is_class<T>::value>
            struct destructor_call
            {
                static void apply(T* obj){}
            };

            template<typename T>
            struct destructor_call<T, true>
            {
                static void apply(T* obj)
                {
                    obj->~T();
                }
            };
            
            static bool move(Variant& v, Variant&& other)
            {
                if(other.tag() == ctti::unnamed_type_id<Head>())
                {
                    new(v.rawStorage()) Head(std::move(other.get<Head>()));

                    v.tag() = other.tag();
                    other.tag() = ctti::unnamed_type_id<void>();

                    return !v.empty() && other.empty();
                }
                else
                {
                    return false;
                }
            }

            static bool copy(Variant& v, const Variant& other)
            {
                if(other.tag() == ctti::unnamed_type_id<Head>())
                {
                    new(v.rawStorage()) Head(other.get<Head>());
                    v.tag() = other.tag();
             
                    return !v.empty();
                }
                else
                {
                    return false;
                }
            }

            static bool move_assign(Variant& v, Variant&& other)
            {
                if(other.tag() == ctti::unnamed_type_id<Head>())
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
                            new (v.rawStorage()) Head( std::move(other.get<Head>()) );
                            v.tag() = ctti::unnamed_type_id<Head>();
                            other.tag() = ctti::unnamed_type_id<void>();

                            return !v.empty() && other.empty();
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
                if(other.tag() == ctti::unnamed_type_id<Head>())
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
                if(v.tag() == ctti::unnamed_type_id<Head>())
                {
                    return f(v.get<Head>());
                }
                else
                {
                    throw std::runtime_error{"Cannot visit " + v.print_variant(v.tag())};
                }
            }
            
            template<typename F>
            static typename F::ResultType visit(Variant& v, F f)
            {
                if(v.tag() == ctti::unnamed_type_id<Head>())
                {
                    return f(v.get<Head>());
                }
                else
                {
                    throw std::runtime_error{ "Cannot visit " + v.print_variant(v.tag())};
                }
            }
        };

        template<typename Head, typename Second, typename... Tail>
        struct VariantExecutorImpl<typelist<Head, Second, Tail...>>
        {
            template<typename T>
            struct has_type_static : public std::integral_constant<bool, 
                    std::is_same<T, Head>::value || VariantExecutor<Second, Tail...>::template has_type_static<T>::value
                >
            {};
            
            static constexpr bool has_type(ctti::unnamed_type_id_t id)
            {
                return VariantExecutor<Head>::has_type(id) && VariantExecutor<Second, Tail...>::has_type(id); 
            }
            
            template<typename... Args>
            static bool construct(Variant& v, Args&&... args)
            {
                return VariantExecutor<Head>::construct(v, std::forward<Args>(args)...) ||
                       VariantExecutor<Second, Tail...>::construct(v, std::forward<Args>(args)...);
            }

            static bool destroy(Variant& v)
            {
                return VariantExecutor<Head>::destroy(v) || VariantExecutor<Second, Tail...>::destroy(v);
            }

            static bool move(Variant& v, Variant&& other)
            {
                return VariantExecutor<Head>::move(v, std::move(other)) || 
                       VariantExecutor<Second, Tail...>::move(v, std::move(other));
            }

            static bool copy(Variant& v, const Variant& other)
            {
                return VariantExecutor<Head>::copy(v, other) ||
                       VariantExecutor<Second, Tail...>::copy(v, other);
            }

            static bool move_assign(Variant& v, Variant&& other)
            {
                return VariantExecutor<Head>::move_assign(v, std::move(other)) ||
                       VariantExecutor<Second, Tail...>::move_assign(v, std::move(other));
            }

            static bool copy_assign(Variant& v, const Variant& other)
            {
                return VariantExecutor<Head>::copy_assign(v, other) ||
                       VariantExecutor<Second, Tail...>::copy_assign(v, other);
            }

            template<typename F>
            static typename F::ResultType const_visit(const Variant& v, F f)
            {
                if(v.tag() == ctti::unnamed_type_id<Head>())
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
                if(v.tag() == ctti::unnamed_type_id<Head>())
                {
                    return f(v.get<Head>());
                }
                else
                {
                    return NextVariantExecutor<Head, Second, Tail...>::visit(v, f);
                }
            }
        };

    public:
        using tag_t = ctti::unnamed_type_id_t;

        const tag_t& tag() const
        {
            return _tag;
        }

        tag_t& tag()
        {
            return _tag;
        }

        std::string to_string() const
        {
            return print_variant(tag());
        }

        bool empty() const
        {
            return _tag == ctti::unnamed_type_id<void>();
        }
        
        explicit operator bool() const
        {
            return !empty();
        }

        void clear()
        {
            VariantExecutor<Ts...>::destroy(*this);
            _tag = ctti::unnamed_type_id<void>();
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

        Variant() = default;

        Variant(const Variant& other)
        {
            auto tag = other.tag();

            if(!other.empty())
            {
                if(!VariantExecutor<Ts...>::copy(*this, other))
                    Throw("copy construct from", tag);
            }
        }

        Variant(Variant&& other) :
            _tag{other.tag()}
        {
            auto tag = other.tag();

            if(!other.empty())
            {
                if(!VariantExecutor<Ts...>::move(*this, std::move(other)))
                    Throw("move-construct from", tag);
            }
        }

        template<typename T, typename = std::enable_if_t<VariantExecutor<Ts...>::template has_type_static<std::decay_t<T>>::value>>
        Variant(T&& value) :
            _tag{ctti::unnamed_type_id<typename std::decay<T>::type>()}
        {
            if(VariantExecutor<Ts...>::template has_type_static<std::decay_t<T>>::value)
                new (rawStorage()) std::decay_t<T>{std::forward<T>(value)};
            else
                Throw("construct from", _tag);
        }

        ~Variant()
        {
            auto tag = this->tag();

            if(!empty())
            {
                if(!VariantExecutor<Ts...>::destroy(*this))
                    Throw("destroy", tag);
            }
        }

        template<typename T, typename = std::enable_if_t<VariantExecutor<Ts...>::template has_type_static<std::decay_t<T>>::value>>
        Variant& operator=(T&& value)
        {
            Variant variant{std::forward<T>(value)};
            return (*this) = std::move(variant);
        }

        Variant& operator=(const Variant& other)
        {
            auto tag = other.tag();

            if(other.empty())
                clear();
            else
                if(!VariantExecutor<Ts...>::copy_assign(*this, other))
                    Throw("copy-assign from", tag);
            
            return *this;
        }

        Variant& operator=(Variant&& other)
        {
            auto tag = other.tag();

            if(other.empty())
            {
                clear();       
            }
            else
            {
                if(!VariantExecutor<Ts...>::move_assign(*this, std::move(other)))
                    Throw("move-assign from", tag);
            }
             
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

        friend bool operator!=(const Variant& lhs, const Variant& rhs)
        {
            return !(lhs == rhs);
        }
 
    private:
        using storage_t = typename std::aligned_union<0, Ts...>::type;
        
        tag_t _tag = ctti::unnamed_type_id<void>();
        storage_t _storage;

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
    };
}

#include <functional>

// std::hash specialization
namespace std
{
    template<typename... Ts>
    struct hash<::cpp::Variant<Ts...>>
    {
        std::size_t operator()(const ::cpp::Variant<Ts...>& v) const
        {
            return v.template visit<std::size_t>(
                [](const auto& elem)
                {
                    return std::hash<std::decay_t<decltype(elem)>>{}(elem);
                }
            );
        }
    };
}
#endif // CEPLUSPLUS_VARIANT_H
