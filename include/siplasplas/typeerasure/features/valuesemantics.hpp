#ifndef SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP
#define SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP

#include <siplasplas/utility/staticif.hpp>
#include <siplasplas/utility/exception.hpp>
#include <siplasplas/utility/tuple.hpp>
#include <siplasplas/utility/destroy.hpp>
#include <siplasplas/utility/memory_manip.hpp>
#include <siplasplas/utility/assert.hpp>
#include <siplasplas/typeerasure/concepts/valuesemantics.hpp>

namespace cpp
{

namespace features
{

class DefaultConstructible
{
public:
    template<typename T>
    static T apply() noexcept(concepts::DefaultConstructible<T>::no_except)
    {
        return cpp::staticIf<concepts::DefaultConstructible<T>::value>([](auto identity)
        {
            return meta::type_t<decltype(identity.template type<T>())>();
        }).Else([](auto) -> T
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not default constructible",
                ctti::type_id<T>().name()
            );
        });
    }

    template<typename T>
    static void apply(void* where) noexcept(concepts::DefaultConstructible<T>::no_except)
    {
        cpp::staticIf<concepts::DefaultConstructible<T>::value>([where](auto identity)
        {
            cpp::construct<meta::type_t<decltype(identity.template type<T>())>>(where);
        }).Else([](auto)
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not default constructible",
                ctti::type_id<T>().name()
            );
        });
    }
};


class Constructible
{
public:
    template<typename T, typename... Args>
    static T apply(Args&&... args) noexcept(concepts::Constructible<T, std::decay_t<Args>...>::no_except)
    {
        // Cannot believe this is the best way to perfect-capture a variadic pack...

        return cpp::staticIf<concepts::Constructible<T, std::decay_t<Args>...>::value>([](auto identity, auto&&... args)
        {
            return meta::type_t<decltype(identity.template type<T>())>(std::forward<decltype(args)>(args)...);
        }, std::forward<Args>(args)...)
        .Else([](auto) -> T
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not constructible",
                ctti::type_id<T>().name()
            );
        });
    }

    template<typename T, typename... Args>
    static void apply(T* where, Args&&... args) noexcept(concepts::Constructible<T, std::decay_t<Args>...>::no_except)
    {
        // Cannot believe this is the best way to perfect-capture a variadic pack...

        cpp::staticIf<concepts::Constructible<T, std::decay_t<Args>...>::value>([where](auto identity, auto&&... args)
        {
            cpp::construct<meta::type_t<decltype(identity.template type<T>())>>(where, std::forward<decltype(args)>(args)...);
        }, std::forward<Args>(args)...)
        .Else([](auto)
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not constructible",
                ctti::type_id<T>().name()
            );
        });
    }

    template<typename T, typename... Args>
    static void apply(void* where, Args&&... args) noexcept(concepts::Constructible<T, std::decay_t<Args>...>::no_except)
    {
        apply<T>(reinterpret_cast<T*>(where), std::forward<Args>(args)...);
    }
};


class CopyConstructible
{
public:
    template<typename T, bool CopyConstructible = concepts::CopyConstructible<T>::value>
    struct Apply
    {
        static T apply(const T& value) noexcept(concepts::CopyConstructible<T>::no_except)
        {
            return T{value};
        }

        static void apply(T* where, const T& value) noexcept(concepts::CopyConstructible<T>::no_except)
        {
            return cpp::construct<T>(where, value);
        }

        static void apply(void* where, const void* other) noexcept(concepts::CopyConstructible<T>::no_except)
        {
            return cpp::construct<T>(where, *reinterpret_cast<const T*>(other));
        }
    };

    template<typename T>
    struct Apply<T, false>
    {
        static T apply(const T& ealue) noexcept(false)
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not copy constructible",
                ctti::type_id<T>().name()
            );
        }

        static void apply(T* where, const T& value) noexcept(false)
        {
            apply(value);
        }

        static void apply(void* where, const void* other) noexcept(false)
        {
            apply(reinterpret_cast<T*>(where), *reinterpret_cast<const T*>(other));
        }
    };

    template<typename T>
    static T apply(const T& value) noexcept(concepts::CopyConstructible<T>::no_except)
    {
        return Apply<T>::apply(value);
    }

    template<typename T>
    static void apply(T* where, const T& value) noexcept(concepts::CopyConstructible<T>::no_except)
    {
        Apply<T>::apply(where, value);
    }

    template<typename T>
    static void apply(void* where, const void* other) noexcept(concepts::CopyConstructible<T>::no_except)
    {
        Apply<T>::apply(where, other);
    }
};


class MoveConstructible
{
public:
    template<typename T, bool MoveConstructible = concepts::MoveConstructible<T>::value>
    class Apply
    {
    public:
        static T apply(T&& value) noexcept(concepts::MoveConstructible<T>::no_except)
        {
            return T(std::move(value));
        }

        static void apply(T* where, T&& value) noexcept(concepts::MoveConstructible<T>::no_except)
        {
            cpp::construct(where, std::move(value));
        }

        static void apply(void* where, void* value) noexcept(concepts::MoveConstructible<T>::no_except)
        {
            cpp::construct<T>(where, std::move(*reinterpret_cast<T*>(value)));
        }
    };

    template<typename T>
    class Apply<T, false>
    {
    public:
        static T& apply(T&& value)
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not move constructible",
                ctti::type_id<T>().name()
            );
        }

        static void apply(T* where, T&& value)
        {
            apply(std::move(value));
        }

        static void apply(void* where, void* value)
        {
            apply(reinterpret_cast<T*>(where), std::move(*reinterpret_cast<T*>(value)));
        }
    };

    template<typename T, typename... Args>
    static auto apply(Args&&... args) noexcept(concepts::MoveConstructible<T>::no_except)
    {
        return Apply<T>::apply(std::forward<Args>(args)...);
    }

    template<typename T>
    static auto apply(T&& value) noexcept(concepts::MoveConstructible<std::decay_t<T>>::no_except)
    {
        return Apply<T>::apply(std::forward<T>(value));
    }
};


class Assignable
{
public:
    template<typename T, typename U>
    static decltype(auto) apply(T& lvalue, U&& value) noexcept(concepts::Assignable<T, std::decay_t<U>>::no_except)
    {
        return cpp::staticIf<concepts::Assignable<T, std::decay_t<U>>::value>([](auto identity, T& lvalue, auto&& value)
        {
            return identity(lvalue) = std::forward<decltype(value)>(value);
        }, lvalue, std::forward<U>(value)).Else([](auto) -> T&
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not assignable to type '{}'",
                ctti::type_id<std::decay_t<U>>().name(),
                ctti::type_id<T>().name()
            );
        });
    }

    template<typename T, typename U>
    static decltype(auto) apply(T* where, U&& value) noexcept(concepts::Assignable<T, std::decay_t<U>>::no_except)
    {
        return apply<T>(*where, std::forward<U>(value));
    }

    template<typename T, typename U>
    static decltype(auto) apply(void* where, U&& value) noexcept(concepts::Assignable<T, std::decay_t<U>>::no_except)
    {
        return apply<T>(reinterpret_cast<T*>(where), std::forward<U>(value));
    }
};


class CopyAssignable
{
public:
    template<typename T>
    static decltype(auto) apply(T& lvalue, const T& value) noexcept(concepts::CopyAssignable<T>::no_except)
    {
        return cpp::staticIf<concepts::CopyAssignable<T>::value>([&lvalue, &value](auto identity) -> decltype(auto)
        {
            return identity(lvalue) = value;
        }).Else([](auto) -> T&
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not copy assignable",
                ctti::type_id<T>().name()
            );
        });
    }

    template<typename T>
    static decltype(auto) apply(T* where, const T& value) noexcept(concepts::CopyAssignable<T>::no_except)
    {
        return apply<T>(*where, value);
    }

    template<typename T>
    static decltype(auto) apply(void* where, const void* other) noexcept(concepts::CopyAssignable<T>::no_except)
    {
        return apply<T>(reinterpret_cast<T*>(where), *reinterpret_cast<const T*>(other));
    }
};


class MoveAssignable
{
public:
    template<typename T>
    static decltype(auto) apply(T& lvalue, std::decay_t<T>&& rvalue) noexcept(concepts::MoveAssignable<std::decay_t<T>>::no_except)
    {
        return cpp::staticIf<concepts::MoveAssignable<std::decay_t<T>>::value>([](auto identity, auto& lvalue, auto&& rvalue) -> decltype(auto)
        {
            return lvalue = std::move(rvalue);
        }, lvalue, std::move(rvalue)).Else([](auto) -> T&
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not move assignable",
                ctti::type_id<T>().name()
            );
        });
    }

    template<typename T>
    static decltype(auto) apply(T* where, T&& value) noexcept(concepts::MoveAssignable<T>::no_except)
    {
        return apply<T>(*where, std::move(value));
    }

    template<typename T>
    static decltype(auto) apply(void* where, void* other) noexcept(concepts::MoveAssignable<T>::no_except)
    {
        return apply<T>(reinterpret_cast<T*>(where), std::move(*reinterpret_cast<T*>(other)));
    }
};


class Destructible
{
public:
    template<typename T>
    static void apply(T& lvalue) noexcept(concepts::Destructible<T>::no_except)
    {
        cpp::staticIf<concepts::Destructible<T>::value>([&lvalue](auto identity)
        {
            cpp::destroy(lvalue);
        }).Else([](auto)
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not destructible",
                ctti::type_id<T>().name()
            );
        });
    }

    template<typename T>
    static decltype(auto) apply(T* where) noexcept(concepts::Destructible<T>::no_except)
    {
        return apply<T>(*where);
    }

    template<typename T>
    static decltype(auto) apply(void* where) noexcept(concepts::Destructible<T>::no_except)
    {
        return apply<T>(reinterpret_cast<T*>(where));
    }
};
}

}

#endif // SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP
