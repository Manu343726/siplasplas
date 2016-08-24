#ifndef SIPLASPLAS_TYPEERASURE_FUNCTION_HPP
#define SIPLASPLAS_TYPEERASURE_FUNCTION_HPP

#include "simpleany.hpp"
#include "anyarg.hpp"
#include "invoke.hpp"
#include "anystorage/deadpool.hpp"
#include "anystorage/fixedsize.hpp"
#include <siplasplas/utility/function_traits.hpp>
#include <siplasplas/utility/staticif.hpp>
#include <siplasplas/utility/compiles.hpp>
#include <siplasplas/utility/exception.hpp>

namespace cpp
{

namespace typeerasure
{

/**
 * \ingroup type-erasure
 * \brief Stores a type-erased callable of any signature and kind
 *
 * The Function class goes one step beyond `std::function` and
 * provides a unique class to store any kind of C++ callable object
 *
 * ``` cpp
 *
 * class MyClass
 * {
 * public:
 *     int method(const std::vector<int>&);
 * };
 * void freeFunction(int i);
 *
 * MyClass object;
 *
 * std::vector<Function> functions = {
 *     &MyClass::method,
 *     [](int i, const std::string& str) {
 *         return std::to_string(i) + str;
 *     },
 *     freeFunction
 * };
 *
 * int i = functions[0](object, std::vector<int>{1, 2, 3, 4});
 * const std::string& str = functions[1](42, "hello");
 * functions[3](42);
 * ```
 *
 * Invoking a Function objects with the wrong arguments has undefined behavior
 *
 * \tparam Storage Storage for the hosted callable
 * \tparam ArgsStorage Storage for type-erased call arguments
 * \tparam ReturnStorage Storage for the return value
 */
template<typename Storage, typename ArgsStorage = Storage, typename ReturnStorage = Storage>
class Function
{
public:
    Function() = default;

    /**
     * \brief Constructs a Function from a Callable object
     *
     * This constructor takes any kind of callable and stores it in a Function.
     * The behavior is undefined if Function is initialized with a type that is
     * not callable.
     *
     * \param callable Callable entity to store. Could be a function pointer,
     * a member function pointer, a lambda expression, etc.
     */
    template<typename Callable>
    Function(Callable&& callable) :
        _invoke{Invoke<std::decay_t<Callable>>{std::forward<Callable>(callable)}}
    {}

    /**
     * \brief Checks if the object is empty (No callable assigned)
     */
    bool empty() const
    {
        return _invoke.empty();
    }

    /**
     * \brief Creates a Function by instancing in-place the given callable
     *
     * \tparam Callable Callable type to store
     * \param args Callable constructor arguments
     *
     * \returns A Function instance with the given callable type
     */
    template<typename Callable, typename... Args>
    static Function<Storage> create(Args&&... args)
    {
        return Function<Storage>{meta::identity<Callable>(), std::forward<Args>(args)...};
    }

    /**
     * \brief Invokes the callable with the given arguments
     *
     * The behavior is undefined if the arguments don't match
     * the underlying callable signature
     *
     * \param args function arguments. If the underlying callable is a pointer to
     * to member function, the first argument is assumed to be the caller object
     * \returns the return value of the callable as in `cpp::invoke(<underlying callable>, std::forward(args)...)`
     */
    template<typename... Args>
    SimpleAny<ReturnStorage> operator()(Args&&... args)
    {
        AnyArg argsArray[] = {std::forward<Args>(args)..., AnyArg(nullptr)};
        return _invoke.template get<InvokeInterface>().invoke(std::begin(argsArray));
    }

    /**
     * \brief Invokes the callable with the given arguments
     *
     * The behavior is undefined if the arguments don't match
     * the underlying callable signature
     *
     * \param args function arguments. If the underlying callable is a pointer to
     * to member function, the first argument is assumed to be the caller object
     * \returns the return value of the callable as in `cpp::invoke(<underlying callable>, std::forward(args)...)`
     */
    template<typename... Args>
    SimpleAny<ReturnStorage> operator()(Args&&... args) const
    {
        AnyArg argsArray[] = {std::forward<Args>(args)..., AnyArg(nullptr)};
        return _invoke.template get<InvokeInterface>().invoke(std::begin(argsArray));
    }

    /**
     * \brief Invokes the callable with the given arguments
     * The arguments are passed as a vector of SimpleAny
     *
     * \param args Forwarding reference to vector of arguments
     *
     * \returns the return value of the callable as in `cpp::invoke(<underlying callable>, <args>)`
     */
    template<typename ArgsVector>
    SimpleAny<ReturnStorage> invoke(ArgsVector&& args)
    {
        return _invoke.template get<InvokeInterface>().invoke(std::forward<ArgsVector>(args));
    }

    /**
     * \brief Invokes the callable with the given arguments
     * The arguments are passed as a vector of SimpleAny
     *
     * \param args Forwarding reference to vector of arguments
     *
     * \returns the return value of the callable as in `cpp::invoke(<underlying callable>, <args>)`
     */
    template<typename ArgsVector>
    SimpleAny<ReturnStorage> invoke(ArgsVector&& args) const
    {
        return _invoke.template get<InvokeInterface>().invoke(std::forward<ArgsVector>(args));
    }

    /**
     * \brief Invokes the callable with the given arguments
     * The arguments are passed as a vector of SimpleAny
     *
     * \param args Pointer to the sequence of arguments
     *
     * \returns the return value of the callable as in `cpp::invoke(<underlying callable>, <args>)`
     */
    SimpleAny<ReturnStorage> invoke(AnyArg* args)
    {
        return _invoke.template get<InvokeInterface>().invoke(args);
    }

    /**
     * \brief Invokes the callable with the given arguments
     * The arguments are passed as a vector of SimpleAny
     *
     * \param args Pointer to the sequence of arguments
     *
     * \returns the return value of the callable as in `cpp::invoke(<underlying callable>, <args>)`
     */
    SimpleAny<ReturnStorage> invoke(AnyArg* args) const
    {
        return _invoke.template get<InvokeInterface>().invoke(args);
    }

    /**
     * \brief Assigns a new callable to the function
     */
    template<typename Callable>
    Function& operator=(Callable&& callable)
    {
        _invoke = Invoke<std::decay_t<Callable>>{std::forward<Callable>(callable)};
        return *this;
    }

    /**
     * \brief Returns the function kind (free function, pointer to member function, functor, etc)
     * of the callable
     */
    cpp::FunctionKind kind() const
    {
        return _invoke.template get<InvokeInterface>().kind();
    }

private:
    template<typename Callable, typename... Args>
    Function(meta::identity<Callable>, Args&&... args) :
        _invoke{Invoke<Callable>{std::forward<Args>(args)...}}
    {}

    class InvokeInterface
    {
    public:
        virtual ~InvokeInterface() = default;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>&& args) = 0;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>&& args) const = 0;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>& args) = 0;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>& args) const = 0;
        virtual SimpleAny<ReturnStorage> invoke(AnyArg* args) = 0;
        virtual SimpleAny<ReturnStorage> invoke(AnyArg* args) const = 0;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>&& args) = 0;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>&& args) const = 0;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>& args) = 0;
        virtual SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>& args) const = 0;
        virtual SimpleAny<ReturnStorage> invoke(SimpleAny<ArgsStorage>* args) = 0;
        virtual SimpleAny<ReturnStorage> invoke(SimpleAny<ArgsStorage>* args) const = 0;

        virtual void* getObject() = 0;
        virtual const void* getObject() const = 0;
        virtual cpp::FunctionKind kind() const = 0;
    };

    template<typename Callable>
    class Invoke : public InvokeInterface
    {
    public:
        template<typename... Args>
        Invoke(Args&&... args) :
            _callable{std::forward<Args>(args)...}
        {}

        SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>&& args) override
        {
            return doInvoke(std::move(args));
        }

        SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>&& args) const override
        {
            return doInvoke(std::move(args));
        }

        SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>& args) override
        {
            return doInvoke(args);
        }

        SimpleAny<ReturnStorage> invoke(std::vector<AnyArg>& args) const override
        {
            return doInvoke(args);
        }

        SimpleAny<ReturnStorage> invoke(AnyArg* args) override
        {
            return doInvoke(args);
        }

        SimpleAny<ReturnStorage> invoke(AnyArg* args) const override
        {
            return doInvoke(args);
        }

        SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>&& args) override
        {
            return doInvoke(std::move(args));
        }

        SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>&& args) const override
        {
            return doInvoke(std::move(args));
        }

        SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>& args) override
        {
            return doInvoke(args);
        }

        SimpleAny<ReturnStorage> invoke(std::vector<SimpleAny<ArgsStorage>>& args) const override
        {
            return doInvoke(args);
        }

        SimpleAny<ReturnStorage> invoke(SimpleAny<ArgsStorage>* args) override
        {
            return doInvoke(args);
        }

        SimpleAny<ReturnStorage> invoke(SimpleAny<ArgsStorage>* args) const override
        {
            return doInvoke(args);
        }

        template<typename Args>
        SimpleAny<ReturnStorage> doInvoke(Args&& args)
        {
            return doInvoke(_callable, std::forward<Args>(args));
        }

        template<typename Args>
        SimpleAny<ReturnStorage> doInvoke(Args&& args) const
        {
            return doInvoke(_callable, std::forward<Args>(args));
        }

#define SIPLASPLAS_FUNCTION_INVOKE_EXPRESSION cpp::typeerasure::invoke(identity(std::forward<Callable_>(callable)), identity(std::forward<Args>(args)))

        template<typename Callable_, typename Args>
        static SimpleAny<ReturnStorage> doInvoke(Callable_&& callable, Args&& args)
        {
            return cpp::staticIf<!(cpp::function_kind<Callable>() == cpp::FunctionKind::MEMBER_FUNCTION && std::is_const<std::remove_reference_t<Args>>::value)>([&](auto identity) -> SimpleAny<ReturnStorage>
            {
                return cpp::staticIf<std::is_void<decltype(SIPLASPLAS_FUNCTION_INVOKE_EXPRESSION)>::value>([&](auto identity) -> SimpleAny<ReturnStorage>
                {
                    SIPLASPLAS_FUNCTION_INVOKE_EXPRESSION;
                    return cpp::SimpleAny<ReturnStorage>();
                }).Else([&](auto identity) -> SimpleAny<ReturnStorage>
                {
                    return SIPLASPLAS_FUNCTION_INVOKE_EXPRESSION;
                });
            })
            .Else([](auto) -> SimpleAny<ReturnStorage>
            {
                throw cpp::exception<std::runtime_error>(
                    "Cannot invoke a non-const member function with a const vector of arguments"
                );
            });

        }

#undef SIPLASPLAS_FUNCTION_INVOKE_EXPRESSION

        void* getObject() override
        {
            return &_callable;
        }

        const void* getObject() const override
        {
            return &_callable;
        }

        cpp::FunctionKind kind() const override
        {
            return cpp::function_kind<Callable>();
        }

    private:
        Callable _callable;
    };

    SimpleAny32 _invoke;
};


/**
 * \ingroup type-erasure
 * \brief A Function with 8 byte fixed-size storage
 */
using Function8  = Function<DeadPoolStorage<8>>;

/**
 * \ingroup type-erasure
 * \brief A Function with 16 byte fixed-size storage
 */
using Function16 = Function<DeadPoolStorage<16>>;

/**
 * \ingroup type-erasure
 * \brief A Function with 32 byte fixed-size storage
 */
using Function32 = Function<DeadPoolStorage<32>>;

/**
 * \ingroup type-erasure
 * \brief A Function with 64 byte fixed-size storage
 */
using Function64 = Function<DeadPoolStorage<64>>;

}

}
#endif // SIPLASPLAS_TYPEERASURE_FUNCTION_HPP
