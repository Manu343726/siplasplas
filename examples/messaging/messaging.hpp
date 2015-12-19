#ifndef SIPLASPLAS_EXAMPLES_MESSAGING_MESSAGING_HPP
#define SIPLASPLAS_EXAMPLES_MESSAGING_MESSAGING_HPP

#include "variant/variant.hpp"
#include "variant/multi_visitor.hpp"

namespace cpp
{
    namespace examples
    {
        template<std::size_t Arity, typename... Args>
        class Message
        {
        public:
            template<typename... Ts>
            Message(Ts&&... args) :
                _args{std::forward<Ts>(args)...}
            {}

            template<typename Visitor>
            void process(Visitor&& visitor) const
            {
                process(std::forward<Visitor>(visitor), std::make_index_sequence<Arity>{});
            }

            template<typename Visitor, std::size_t... Is>
            void process(Visitor&& visitor, std::index_sequence<Is...>) const
            {
                return cpp::multi_visitor<void>(
                    std::forward<Visitor>(visitor)
                )(_args[Is]...);
            }

        private:
            std::array<::cpp::Variant<Args...>, Arity> _args;
        };

        template<typename T, std::size_t Arity, typename... Args>
        struct MessageHandler
        {
            template<typename...> class list {};
            template<typename...> using void_t = void;

            // processor class is a dispatcher to the T::process() function.
            // It just asks "Has T a process() overload to handle this event?"
            // If the answer is "yes", calls it, else does nothing.
            template<typename U, typename _Args, typename = void>
            struct processor;

            template<typename U, typename... _Args>
            struct processor<U, list<_Args...>, void_t<decltype(std::declval<U>().process(std::declval<std::decay_t<Args>>()...))>>
            {
                static auto apply(U* This, _Args&&... args)
                {
                    This->process(std::forward<Args>(args)...);          
                }
            };

            template<typename U, typename... _Args, typename _void>
            struct processor<U, list<_Args...>, _void>
            {
                // Note a call to an empty function means no call at runtime
                static void apply(U* This, _Args&&... args)
                {
                    std::cout << "nop!" << std::endl;
                }
            };

            template<typename... _Args>
            void apply_processor(_Args&&... args) const
            {
                processor<const T, list<_Args...>>::apply(static_cast<const T*>(this),
                        std::forward<_Args>(args)...);
            }

            void receive(const Message<Arity, Args...>& message) const
            {
                message.process(
                    [this](auto&&... args)
                    {
                        this->apply_processor(std::forward<decltype(args)>(args)...);
                    }
                ); 
            }
        };

        namespace detail 
        {
            template<typename Class, typename Message>
            class HandlerFor;

            template<typename Class, std::size_t Arity, typename... Args>
            struct HandlerFor<Class, Message<Arity, Args...>>
            {
                using type = MessageHandler<Class, Arity, Args...>;   
            };
        }

        template<typename Class, typename Message>
        using HandlerFor = typename detail::HandlerFor<Class, Message>::type;
    }
}

#endif // SIPLASPLAS_EXAMPLES_MESSAGING_MESSAGING_HPP
