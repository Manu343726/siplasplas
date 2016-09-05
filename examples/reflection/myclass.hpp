#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
#define SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP

#include <siplasplas/reflection/api.hpp>
#include <siplasplas/reflection/dynamic/attributes/contract.hpp>

#include <siplasplas/utility/meta.hpp>

namespace meta = cpp::meta;

class Bind : public cpp::dynamic_reflection::attributes::Attribute
{
public:
    template<typename... Args>
    Bind(Args&&... args) :
        _bindedArgs{std::make_pair(std::is_placeholder<std::decay_t<Args>>::value, std::forward<Args>(args))...}
    {}

    std::vector<cpp::dynamic_reflection::Object> processArguments(const std::vector<cpp::dynamic_reflection::Object>& args)
    {
        const std::size_t function_arity = _bindedArgs.size();

        std::vector<cpp::dynamic_reflection::Object> finalArgs;
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
    std::vector<std::pair<int, cpp::dynamic_reflection::Object>> _bindedArgs;
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
    $(cpp::drfl::attributes::contract(
        [](int a, int b){ return a > 0; }, "a must be greater than zero",
        [](int r){ return true; },         "this must pass always"
    ))
    int f(int a, int b)
    {
        return a + b;
    }

    int g(int a, int b, int c, int d)
    {
        return a + b + c + d;
    }

    $(enable_reflection)
    class InnerClass
    {
    public:
        int innerMember = 0;
    };

    enum class Enum
    {
        ENUM_VALUE_1 = 1,
        ENUM_VALUE_2 = 2,
        FOO,
        BAR,
        QUUX
    };

    int field = 0;
    bool otherField;
    InnerClass objectOfInnerClass;
    std::string strField = "hello";

    std::vector<std::string> vector = {"hello", ", ", "world!"};
    std::unordered_map<std::string, int> answers = {
        {"everything", 42},
        {"everything squared", 42*42}
    };
    std::tuple<int, std::string, InnerClass> tuple = std::make_tuple(
        42,
        "everything",
        InnerClass()
    );
};

#include <reflection/examples/reflection/myclass.hpp>

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
