#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP

#include "entity.hpp"
#include <siplasplas/typeerasure/function.hpp>

namespace cpp
{

namespace dynamic_reflection
{

/**
 * \ingroup dynamic-reflection
 * \brief Stires dynamic reflection information of a function
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Function : public Entity
{
public:
    /**
     * \brief Invokes the function with the given arguments
     */
    template<typename... Args>
    auto operator()(Args&&... args) const
    {
        return _functionPointer(std::forward<Args>(args)...);
    }

    /**
     * \brief Invokes the function with the given arguments
     */
    template<typename... Args>
    auto operator()(Args&&... args)
    {
        return _functionPointer(std::forward<Args>(args)...);
    }

    /**
     * \brief Returns a pointer to the function
     */
    const cpp::typeerasure::Function32& getFunction() const;

    /**
     * \brief Instances a function entity given its source information and a function pointer
     *
     * \param sourceInfo Field source information
     * \param function Function pointer. Must be non-empty.
     *
     * \returns A shared pointer holding the function entity
     */
    static std::shared_ptr<Function> create(const SourceInfo& sourceInfo, const cpp::typeerasure::Function32& function);

    /**
     * \brief Returns a reference to a function entity from a pointer to an entity
     *
     * This function performs an entity cast from a generic entity pointer to a
     * function entity. The behavior is undefined if the entity is not a function
     * (See Entity::kind())
     *
     * \param entity A shared pointer to an entity
     *
     * \returns A reference to a field entity
     */
    static Function& fromEntity(const std::shared_ptr<Entity>& entity);

private:
    Function(const SourceInfo& sourceInfo, const cpp::typeerasure::Function32& function);

    cpp::typeerasure::Function32 _functionPointer;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
