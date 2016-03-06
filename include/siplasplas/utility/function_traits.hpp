#ifndef SIPLASPLAS_UTILITY_FUNCTION_TRAITS_HPP
#define SIPLASPLAS_UTILITY_FUNCTION_TRAITS_HPP

#include "meta.hpp"
#include <type_traits>

namespace cpp
{

namespace detail
{
	template<typename Function>
	struct get_function_signature;

	template<typename R, typename... Args>
	struct get_function_signature<R(Args...)>
	{
		using args = meta::list<Args...>;
		using return_type = R;
	};

	template<typename C, typename R, typename... Args>
	struct get_function_signature<R C::*(Args...)> : 
		get_function_signature<R(Args...)>
	{};

	template<typename C, typename R, typename... Args>
	struct get_function_signature<R C::*(Args...) const> : 
		get_function_signature<R(Args...)>
	{};
}

	template<typename Function, bool = std::is_class<Function>::value>
	struct function_signature : 
		detail::get_function_signature<Function>
	{};

	template<typename Functor>
	struct function_signature<Functor, true> :
		detail::get_function_signature<decltype(&Functor::operator())>
	{};

}

#endif // SIPLASPLAS_UTILITY_FUNCTION_TRAITS_HPP