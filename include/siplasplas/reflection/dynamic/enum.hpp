#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_ENUM_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_ENUM_HPP

#include "entity.hpp"
#include <siplasplas/typeerasure/typeinfo.hpp>
#include <siplasplas/typeerasure/simpleany.hpp>

namespace cpp
{

namespace dynamic_reflection
{

/**
 * \ingroup dynamic-reflection
 * \brief Stores dynamic reflection information of an enumeration type
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Enum : public Entity
{
public:
    /**
     * \brief Instantiates an enumeration entity given its source information,
     * its type information, and the enumeration constants
     *
     * \param sourceInfo Enumeration source information
     * \param type Enumeration type information
     * \param underlyingType Enumeration underlying type type information
     * \param constantsNames Enumeration constants names
     * \param Enumeration constants values
     *
     * \returns A shared pointer holding the enumeration entity. The returned entity is detached
     * (See cpp::dynamic_reflection::Runtime::addEntity())
     */
    static std::shared_ptr<Enum> create(
        const SourceInfo& sourceInfo,
        const cpp::typeerasure::TypeInfo& type,
        const cpp::typeerasure::TypeInfo& underlyingType,
        const std::vector<std::string>& constantsNames,
        const std::vector<std::int64_t>& constantsValues
    );

    /**
     * \brief Returns a reference to an enumeration entity from a pointer to an entity
     *
     * This function performs an entity cast from a generic entity pointer to a
     * enumeration entity. The behavior is undefined if the entity is not an enumeration
     * (See Entity::kind())
     *
     * \param entity A shared pointer to an entity
     *
     * \returns A reference to a enumeration entity
     */
    static Enum& fromEntity(const std::shared_ptr<Entity>& entity);

    /**
     * \brief Returns the number of constants in the enumeration
     */
    std::size_t count() const;

    /**
     * \brief Returns the name of the i-th enumeration constant
     *
     * \param i Index of the requested enumeration constant. Constants are indexed
     * from top/left from bottom/right, starting at zero. The behavior is undefined if i >= count()
     */
    const std::string& name(std::size_t i) const;

    /**
     * \brief Returns the set of enum constants names
     */
    const std::vector<std::string>& names() const;

    /**
     * \brief Returns the value of the i-th enumeration constant
     *
     * Enumeration values are converted to signed 64 bit integers to be stored
     * in the Enum object. Users can check the original underlying type with
     * underlyingTypeInfo(), and check if the original values were unsigned with
     * isUnsigned().
     *
     * \param i Index of the requested enumeration constant. Constants are indexed
     * from top/left from bottom/right, starting at zero. The behavior is undefined if i >= count()
     */
    std::int64_t value(std::size_t i) const;

    /**
     * \brief Returns the set of enum constants values
     */
    const std::vector<std::int64_t>& values() const;

    /**
     * \brief Checks whether the underlying enumeration type is an unsigned integral type
     *
     * \returns True if the enumeration constants values are unsigned integers, false otherwise
     */
    bool isUnsigned() const;

    /**
     * \brief Returns the enumeration value of the constant with the given name
     *
     * \param name Enumeration constant name. The behavior is undefined if the enum has no
     * constant with the given name
     */
    std::int64_t fromString(const std::string& name) const;

    /**
     * \brief Returns The name of the enumeration constant with the given value
     *
     * \param value Enumeration constant value. The behavior is undefined if the enum has no
     * constant with that value. If there are multiple constants with that value, the first
     * is returned
     */
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
