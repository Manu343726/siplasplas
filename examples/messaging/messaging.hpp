#ifndef SIPLASPLAS_EXAMPLES_MESSAGING_MESSAGING_HPP
#define SIPLASPLAS_EXAMPLES_MESSAGING_MESSAGING_HPP

#include "variant/variant.hpp"
#include "variant/multi_visitor.hpp"

#include "utility/function.hpp"

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
			template<typename...> struct List {};
			template<typename...> using void_t = void;

			template<typename Class, typename CallArgs, typename = void>
			struct CallProcess
			{
				template<typename... _Args>
				static void apply(Class* This, _Args&&... args)
				{
				
				}
			};

			template<typename Class, typename... _Args>
			struct CallProcess<Class, List<_Args...>, decltype(std::declval<Class>().process(std::declval<_Args>()...))>
			{
				template<typename... __Args>
				static void apply(Class* This, __Args&&... args)
				{
					return This->process(std::forward<__Args>(args)...);
				}
			};

 			void receive(const Message<Arity, Args...>& message) const
            {
                message.process(
                    [this](auto&&... args)
                    {
						using CallArgs = List<decltype(std::forward<decltype(args)>(args))...>;


						return CallProcess<const T, CallArgs>::apply(static_cast<const T*>(this), std::forward<decltype(args)>(args)...);
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
