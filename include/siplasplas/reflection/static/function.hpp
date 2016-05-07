#ifndef SIPLASPLAS_REFLECTION_STATIC_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_STATIC_FUNCTION_HPP

namespace cpp
{
namespace static_reflection
{
namespace meta
{

template<typename AstInfo, typename FunctionType, FunctionType function>
class Function;

template<typename AstInfo, typename R, typename... Args,
    R(*function)(Args...)>
class Function<AstInfo, R(*)(Args...), function> : public AstInfo
{
public:
    using type = R(*)(Args...);

    constexpr Function() = default;

    template<typename... Args_>
    static constexpr R invoke(Args_&&... args)
    {
        return function(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    constexpr R operator()(Args_&&... args) const
    {
        return invoke(std::forward<Args_>(args)...);
    }

    static constexpr type get()
    {
        return function;
    }
};

template<typename AstInfo, typename R, typename Class, typename... Args,
    R(Class::*method)(Args...) const>
class Function<AstInfo, R(Class::*)(Args...) const, method> : public AstInfo
{
public:
    using type = R(Class::*)(Args...) const;

    constexpr Function() = default;

    template<typename... Args_>
    static constexpr R invoke(const Class& object, Args_&&... args)
    {
        return object.*method(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    constexpr R operator()(const Class& object, Args_&&... args) const
    {
        return invoke(object, std::forward<Args_>(args)...);
    }

    static constexpr type get()
    {
        return method;
    }
};

template<typename AstInfo, typename R, typename Class, typename... Args,
    R(Class::*method)(Args...)>
class Function<AstInfo, R(Class::*)(Args...), method> : public AstInfo
{
public:
    using type = R(Class::*)(Args...);

    constexpr Function() = default;

    template<typename... Args_>
    static constexpr R invoke(Class& object, Args_&&... args)
    {
        return object.*method(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    static constexpr R invoke(const Class& object, Args_&&... args)
    {
        return object.*method(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    constexpr R operator()(Class& object, Args_&&... args) const
    {
        return invoke(object, std::forward<Args_>(args)...);
    }

    static constexpr type get()
    {
        return method;
    }
};

template<typename... Fs>
class OverloadedFunction;

template<typename Head>
class OverloadedFunction<Head> : public Head
{};

template<typename Head, typename Second, typename... Tail>
class OverloadedFunction<Head, Second, Tail...> : public Head, public OverloadedFunction<Second, Tail...>
{
public:
    using Head::invoke;
    using Head::operator();
    using OverloadedFunction<Second, Tail...>::invoke;
    using OverloadedFunction<Second, Tail...>::operator();
};

} // namespace meta

namespace codegen
{

template<typename FunctionType, FunctionType function>
class Function :
    public static_reflection::meta::Function<
        static_reflection::meta::EmptyAstInfo<Function<FunctionType, function>>,
        FunctionType,
        function
    >
{};

}

template<typename FunctionType, FunctionType function>
class Function : public codegen::Function<FunctionType, function>
{};

} // namespace static_reflection
} // namespace cpp
#endif // SIPLASPLAS_REFLECTION_STATIC_FUNCTION_HPP
