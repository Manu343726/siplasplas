#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_SOURCEINFO_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_SOURCEINFO_HPP

#include "scope.hpp"
#include <siplasplas/reflection/static/sourceinfo.hpp>
#include <string>

namespace cpp
{

namespace dynamic_reflection
{

using Kind = static_reflection::Kind;

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT SourceInfo
{
public:
    using Kind = dynamic_reflection::Kind;

    SourceInfo(const std::string& fullName,
               const Kind& kind,
               const std::string& spelling,
               const std::string& displayName,
               const std::string& file,
               std::size_t line);

    SourceInfo(const std::string& fullName, const Kind& kind);

    template<typename Entity>
    static SourceInfo fromStaticSourceInfo()
    {
        return {
            Entity::SourceInfo::SourceInfo::fullName().str(),
            Entity::SourceInfo::kind(),
            Entity::SourceInfo::spelling().str(),
            Entity::SourceInfo::displayName(),
            Entity::SourceInfo::file(),
            Entity::SourceInfo::line()
        };
    }

    const std::string& spelling() const;
    const std::string& displayName() const;
    const std::string& file() const;
    std::size_t line() const;

    const std::string& name() const;
    const std::string& fullName() const;

    const Scope& scope() const;
    const Kind& kind() const;

private:
    Scope _scope;
    Kind _kind;
    std::string _spelling;
    std::string _displayName;
    std::string _file;
    std::size_t _line;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_SOURCEINFO_HPP
