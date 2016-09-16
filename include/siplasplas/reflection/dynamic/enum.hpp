#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_ENUM_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_ENUM_HPP

#include "entity.hpp"
#include <siplasplas/typeerasure/typeinfo.hpp>
#include <siplasplas/typeerasure/simpleany.hpp>

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Enum : public Entity
{
public:
    static std::shared_ptr<Enum> create(
        const SourceInfo& sourceInfo,
        const cpp::typeerasure::TypeInfo& type,
        const cpp::typeerasure::TypeInfo& underlyingType,
        const std::vector<std::string>& constantsNames,
        const std::vector<std::int64_t>& constantsValues
    );

    static Enum& fromEntity(const std::shared_ptr<Entity>& entity);

    std::size_t count() const;
    const std::string& name(std::size_t i) const;
    const std::vector<std::string>& names() const;
    std::int64_t value(std::size_t i) const;
    const std::vector<std::int64_t>& values() const;

    bool isUnsigned() const;

    std::int64_t fromString(const std::string& name) const;
    const std::string& toString(std::int64_t value) const;

private:
    Enum(
        const SourceInfo& sourceInfo,
        const cpp::typeerasure::TypeInfo& type,
        const cpp::typeerasure::TypeInfo& underlyingType,
        const std::vector<std::string>& constantsNames,
        const std::vector<std::int64_t>& constantsValues
    );

    cpp::typeerasure::TypeInfo _type;
    cpp::typeerasure::TypeInfo _underlyingType;
    std::vector<std::string>  _names;
    std::vector<std::int64_t> _values;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_ENUM_HPP
