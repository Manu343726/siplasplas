#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_API_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_API_HPP

#include "type.hpp"
#include "object.hpp"
#include "object_manip.hpp"
#include "field.hpp"
#include "function.hpp"
#include "class.hpp"
#include "enum.hpp"
#include "namespace.hpp"
#include "runtime.hpp"

#include <siplasplas/reflection/common/annotations.hpp>

/**
 * \ingroup reflection
 * \defgroup dynamic-reflection
 * \brief Access to dynamic reflection information
 *
 * # Examples:
 *
 *  - \link examples/reflection/dynamic/plugin.cpp\endlink
 *  - \link examples/reflection/dynamic/reflection.cpp\endlink
 */

/**
 * \ingroup dynamic-reflection
 * \example examples/reflection/dynamic/plugin.cpp
 *
 * This example shows how the siplasplas-reflection-dynamic
 * API can be used to export library API types and functions,
 * and be loaded at runtime after loading the library
 */

/**
 * \ingroup dynamic-reflection
 * \example examples/reflection/dynamic/reflection.cpp
 *
 * SHows the basics of the dynamic reflection API: How to
 * register entities, reference types, instance objects,
 * etc.
 */

namespace cpp
{

namespace drfl = ::cpp::dynamic_reflection;

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_API_HPP
