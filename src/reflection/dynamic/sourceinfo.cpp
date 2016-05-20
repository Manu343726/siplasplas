#include "sourceinfo.hpp"
#include <cassert>

using namespace cpp;
using namespace cpp::dynamic_reflection;

SourceInfo::SourceInfo(const std::string& fullName,
                       const SourceInfo::Kind& kind,
                       const std::string& spelling,
                       const std::string& displayName,
                       const std::string& file,
                       std::size_t line) :
    _scope{Scope::fromFullName(fullName)},
    _kind{kind},
    _spelling{spelling},
    _displayName{displayName},
    _file{file},
    _line{line}
{}

SourceInfo::SourceInfo(const std::string& fullName, const SourceInfo::Kind& kind) :
    _scope{Scope::fromFullName(fullName)},
    _kind{kind},
    _spelling{_scope.name()},
    _displayName{_scope.name()},
    _file{""},
    _line{0}
{}

const std::string& SourceInfo::fullName() const
{
    return _scope.fullName();
}

const std::string& SourceInfo::spelling() const
{
    return _spelling;
}

const std::string& SourceInfo::displayName() const
{
    return _displayName;
}

const std::string& SourceInfo::file() const
{
    return _file;
}

std::size_t SourceInfo::line() const
{
    return _line;
}

const std::string& SourceInfo::name() const
{
    return _spelling;
}

const SourceInfo::Kind& SourceInfo::kind() const
{
    return _kind;
}

const Scope& SourceInfo::scope() const
{
    return _scope;
}
