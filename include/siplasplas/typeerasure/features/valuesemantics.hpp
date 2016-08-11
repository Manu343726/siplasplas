#ifndef SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP
#define SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP

#include <siplasplas/utility/staticif.hpp>
#include <siplasplas/utility/exception.hpp>
#include <siplasplas/utility/tuple.hpp>
#include <siplasplas/utility/destroy.hpp>
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
};


class Constructible
{
public:
    template<typename T, typename... Args>
    static T apply(Args&&... args) noexcept(concepts::Constructible<T, std::decay_t<Args>...>::no_except)
    {
        // Cannot believe this is the best way to perfect-capture a variadic pack...

        return cpp::staticIf<concepts::Constructible<T, std::decay_t<Args>...>::value>([args = std::forward_as_tuple(std::forward<Args>(args)...)](auto identity)
        {
            return cpp::tuple_call(args, [](auto... args)
            {
                return meta::type_t<decltype(identity.template type<T>())>(std::forward<decltype(args)>(args)...);
            });
        }).Else([](auto) -> T
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not constructible",
                ctti::type_id<T>().name()
            );
        });
    }
};


class CopyConstructible
{
public:
    template<typename T>
    static T apply(const T& value) noexcept(concepts::CopyConstructible<T>::no_except)
    {
        return cpp::staticIf<concepts::CopyConstructible<T>::value>([&value](auto identity)
        {
            return meta::type_t<decltype(identity.template type<T>())>(value);
        }).Else([](auto) -> T
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not copy constructible",
                ctti::type_id<T>().name()
            );
        });
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
    };

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
    static auto apply(T& lvalue, U&& value) noexcept(concepts::Assignable<T, std::decay_t<U>>::no_except)
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
};


class CopyAssignable
{
public:
    template<typename T>
    static auto apply(T& lvalue, const T& value) noexcept(concepts::CopyAssignable<T>::no_except)
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
    static decltype(auto) apply(T& lvalue, std::decay_t<T>&& rvalue) noexcept(concepts::MoveAssignable<T>::no_except)
    {
        return cpp::staticIf<concepts::MoveAssignable<T>::value>([](auto identity, auto& lvalue, auto&& rvalue) -> decltype(auto)
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
};
}

}

#endif // SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP
