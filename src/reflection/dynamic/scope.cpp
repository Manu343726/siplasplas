#include "scope.hpp"
#include <sstream>

using namespace cpp;
using namespace cpp::dynamic_reflection;

static std::vector<std::size_t> getSeparators(const std::string& fullName)
{
    const std::size_t length = fullName.length();
    std::vector<std::size_t> separators;

    if(length > 0)
    {
        for(std::size_t i = 0; i < length - 1; ++i)
        {
            if(fullName[i] == ':' && fullName[i+1] == ':')
            {
                separators.push_back(i);
                i++;
            }
        }
    }

    return separators;
}

Scope::Scope(const std::string& fullName) :
    _fullName{fullName},
    _separators{getSeparators(fullName)}
{}

Scope::Scope(std::string&& fullName, std::vector<std::size_t>&& separators) :
    _fullName{std::move(fullName)},
    _separators{std::move(separators)}
{}

Scope Scope::fromFullName(const std::string& fullName)
{
    return {fullName};
}

const Scope& Scope::globalScope()
{
    static Scope globalScope = Scope::fromFullName("");
    return globalScope;
}

Scope Scope::fromParentScope(const Scope& parent, const std::string& name)
{
    std::vector<std::size_t> separators = parent._separators;
    std::ostringstream os;

    return {
        (os << parent.fullName() << "::" << name, os.str()),
        (separators.push_back(parent.fullName().length()), std::move(separators))
    };
}

Scope Scope::parent() const
{
    if(!isGlobalScope())
    {
        return {
            {_fullName.begin(), _fullName.begin() + _separators.back()},
            {_separators.begin(), _separators.begin() + _separators.size() - 1}
        };
    }
    else
    {
        throw std::runtime_error{"Global scope has no parent scope"};
    }
}

std::string Scope::name() const
{
    if(isGlobalScope())
    {
        return "";
    }
    else
    {
        return {_fullName.begin() + _separators.back() + 2, _fullName.end()};
    }
}

const std::string& Scope::fullName() const
{
    return _fullName;
}

std::size_t Scope::depth() const
{
    return _separators.size();
}

bool Scope::isGlobalScope() const
{
    return depth() == 0;
}

namespace cpp
{

namespace dynamic_reflection
{

bool operator==(const Scope& lhs, const Scope& rhs)
{
    return lhs.fullName() == rhs.fullName();
}

bool operator!=(const Scope& lhs, const Scope& rhs)
{
    return !(lhs == rhs);
}

}

}
