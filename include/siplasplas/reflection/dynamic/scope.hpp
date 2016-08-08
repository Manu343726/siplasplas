#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_SCOPE_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_SCOPE_HPP

#include <siplasplas/utility/string.hpp>
#include <siplasplas/reflection/dynamic/export.hpp>

#include <string>
#include <vector>


namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Scope
{
public:
    static const Scope& globalScope();
    static Scope fromFullName(const std::string& fullName);
    static Scope fromParentScope(const Scope& parentScope, const std::string& name);

    std::size_t depth() const;
    std::string name() const;
    const std::string& fullName() const;

    friend bool operator==(const Scope& lhs, const Scope& rhs);
    friend bool operator!=(const Scope& lhs, const Scope& rhs);

    Scope parent() const;
    bool isGlobalScope() const;

private:
    std::string _fullName;
    std::vector<std::size_t> _separators;

    Scope(const std::string& fullName);
    Scope(std::string&& fullName, std::vector<std::size_t>&& separators);
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_SCOPE_HPP
