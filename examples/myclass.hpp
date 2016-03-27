#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
#define SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP

#include <siplasplas/reflection/api.hpp>
#include <siplasplas/reflection/attributes/contract.hpp>

#include <siplasplas/utility/meta.hpp>

namespace meta = cpp::meta;

class Bind : public cpp::attributes::Attribute
{
public:
    template<typename... Args>
    Bind(Args&&... args) :
        _bindedArgs{std::make_pair(std::is_placeholder<std::decay_t<Args>>::value, std::forward<Args>(args))...}
    {}

    std::vector<cpp::MetaObject> processArguments(const std::vector<cpp::MetaObject>& args)
    {
        const std::size_t function_arity = _bindedArgs.size();

        std::vector<cpp::MetaObject> finalArgs;
        finalArgs.reserve(function_arity);

        for(auto&& arg : _bindedArgs)
        {
            if(arg.first > 0)
            {
                finalArgs.emplace_back(args[arg.first - 1]);
            }
            else
            {
                finalArgs.emplace_back(arg.second);
            }
        }

        return finalArgs;
    }

private:
    std::vector<std::pair<int, cpp::MetaObject>> _bindedArgs;
};

namespace attr
{
    template<typename... Args>
    auto bind(Args&&... args)
    {
        return std::make_shared<Bind>(std::forward<Args>(args)...);
    }
}


using namespace std::placeholders;

$(enable_reflection)
class MyClass
{
public:
    $(cpp::attributes::contract(
        [](int a, int b){ return a > 0; }, "a must be greater than zero",
        [](int r){ return true; },         "this must pass always"
    ))
    int f(int a, int b)
    {
        return a + b;
    }

    $(attr::bind(_1, 2, _2, 4))
    int g(int a, int b, int c, int d)
    {
        return a + b + c + d;
    }

    int field = 0;
};

#include <reflection/examples/myclass.hpp>

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
