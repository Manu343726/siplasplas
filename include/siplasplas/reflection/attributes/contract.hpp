#ifndef SIPLASPLAS_REFLECTION_ATTRIBUTES_CONTRACT_HPP
#define SIPLASPLAS_REFLECTION_ATTRIBUTES_CONTRACT_HPP

#include "attribute.hpp"

namespace cpp
{

namespace attributes
{

template<typename ReturnType, typename Args>
class Contract;

template<typename ReturnType, typename... Args>
class Contract<ReturnType, meta::list<Args...>> : public ImplementAttribute<Contract<ReturnType, meta::list<Args...>>>
{
public:
    using PrecondType = std::function<bool(Args...)>;
    using PostCondType = std::function<bool(const ReturnType)>;


    Contract(const PrecondType& precond, const std::string& precondMsg,
             const PostCondType& postcond, const std::string& postcondMsg) :
        _precond{precond},
        _postcond{postcond},
        _precondMsg{precondMsg},
        _postcondMsg{postcondMsg}
    {}

    std::tuple<Args...> processArguments(Args... args)
    {
        if(!_precond(args...))
        {
            throw std::logic_error{_precondMsg};
        }            
        else
        {
            return std::make_tuple(args...);
        }
    }

    ReturnType processReturnValue(ReturnType returnValue)
    {
        if(!_postcond(returnValue))
        {
            throw std::logic_error{_postcondMsg};
        }
        else
        {
            return returnValue;
        }
    }

private:
    std::function<bool(Args...)> _precond;
    std::function<bool(const ReturnType&)> _postcond;
    std::string _precondMsg;
    std::string _postcondMsg;
};

template<typename Precond, typename Postcond>
std::shared_ptr<cpp::attributes::Attribute> contract(Precond precond, const std::string& precondMsg,
         Postcond postcond, const std::string& postcondMsg)
{
    return std::make_shared<
        Contract<cpp::function_argument<0, Postcond>, cpp::function_arguments<Precond>>
    >(precond, precondMsg, postcond, postcondMsg);
}

}

}

#endif // SIPLASPLAS_REFLECTION_ATTRIBUTES_CONTRACT_HPP
