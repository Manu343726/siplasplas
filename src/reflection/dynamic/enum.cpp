#include "enum.hpp"

using namespace cpp;
using namespace cpp::dynamic_reflection;

Enum::Enum(
    const SourceInfo& sourceInfo,
    const cpp::typeerasure::TypeInfo& type,
    const cpp::typeerasure::TypeInfo& underlyingType,
    const std::vector<std::string>& constantsNames,
    const std::vector<std::int64_t>& constantsValues
) :
    Entity{sourceInfo},
    _type{type},
    _underlyingType{underlyingType},
    _names{constantsNames},
    _values{constantsValues}
{
    SIPLASPLAS_ASSERT_FALSE(_names.empty());
    SIPLASPLAS_ASSERT_FALSE(_values.empty());
    SIPLASPLAS_ASSERT_EQ(_names.size(), _values.size());
}

std::shared_ptr<Enum> Enum::create(
    const SourceInfo& sourceInfo,
    const cpp::typeerasure::TypeInfo& type,
    const cpp::typeerasure::TypeInfo& underlyingType,
    const std::vector<std::string>& constantsNames,
    const std::vector<std::int64_t>& constantsValues
)
{
    return std::shared_ptr<Enum>{ new Enum{
        sourceInfo,
        type,
        underlyingType,
        constantsNames,
        constantsValues
    }};
}

Enum& Enum::fromEntity(const std::shared_ptr<Entity>& entity)
{
    if(entity->kind() == SourceInfo::Kind::ENUM)
    {
        return static_cast<Enum&>(*entity);
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' is not an enum",
            entity->fullName()
        );
    }
}

std::size_t Enum::count() const
{
    return _names.size();
}

const std::string& Enum::name(std::size_t i) const
{
    SIPLASPLAS_ASSERT_LT(i, count());

    return _names[i];
}

const std::vector<std::string>& Enum::names() const
{
    return _names;
}

const std::vector<std::int64_t>& Enum::values() const
{
    return _values;
}

std::int64_t Enum::value(std::size_t i) const
{
    SIPLASPLAS_ASSERT_LT(i, count());

    return _values[i];
}

bool Enum::isUnsigned() const
{
    return _underlyingType(cpp::TypeTrait::is_unsigned);
}

std::int64_t Enum::fromString(const std::string& name) const
{
    for(std::size_t i = 0; i < count(); ++i)
    {
        if(name == _names[i])
        {
            return _values[i];
        }
    }

    throw cpp::exception<std::domain_error>(
        "Enum '{}' has no constant named '{}'",
        fullName(),
        name
    );
}

const std::string& Enum::toString(std::int64_t value) const
{
    for(std::size_t i = 0; i < count(); ++i)
    {
        if(value == _values[i])
        {
            return _names[i];
        }
    }

    throw cpp::exception<std::domain_error>(
        "Enum '{}' has no constant with value {}",
        fullName(),
        value
    );
}
