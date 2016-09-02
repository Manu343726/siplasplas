#ifndef SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP

#include "sourceinfo.hpp"
#include <siplasplas/utility/meta.hpp>

namespace cpp
{

namespace static_reflection
{

namespace meta
{

/**
 * \ingroup static-reflection
 * \brief Returns information of a given class
 */
template<typename SourceInfo_, typename Class_,
    typename Methods_,
    typename Fields_,
    typename Constructors_,
    typename Classes_,
    typename Enums_
>
class Class
{
public:
    using class_type = Class_;
    using type = class_type;

    /**
     * \brief Returns the source information of the class 
     *
     * Returns a cpp::static_reflection::SourceInfo instance with the
     * source information of the declaration of the class. This information includes:
     *  - Class name, e.g. "MyClass". See cpp::static_reflection::SourceInfo::spelling()
     *  - Full qualified class name, e.g. "MyNamespace::MyClass". See cpp::static_reflection::SourceInfo::fullName()
     *  - Full path to the file where the class is declared, e.g. "/home/user/myproject/include/MyLib/MyLib.h". See cpp::static_reflection::SourceInfo::file()
     *  - Line of the file where the class is declared, e.g. 42. See cpp::static_reflection::SourceInfo::line()
     */
    using SourceInfo = SourceInfo_;

    /**
     * \brief Returns a list with the information of the public non-static methods
     * of the class.
     *
     * Methods is implemented as an alias to a cpp::meta::list<> with one cpp::static_reflection::Function
     * instance for each public non-static member function of the class.
     */
    using Methods = Methods_;

    /**
     * \brief Returns a list with the information of the public non-static member objects
     * of the class.
     *
     * Fields is implemented as an alias to a cpp::meta::list<> with one cpp::static_reflection::Field
     * instance for each public non-static member object of the class.
     */
    using Fields = Fields_;
    
    /**
     * \brief Returns information about the public constructors of the class. **Returns an empty cpp::meta::list<>, constructor information
     * is not currently collected by the reflection parser**
     */
    using Constructors = Constructors_;

    /**
     * \brief Returns the list of member classes of the class
     *
     * Classes is implemented as an alias to a cpp::meta::list<> instance with the member class types of the class
     */
    using Classes = Classes_;

    /**
     * \brief Returns the list of member enums of the class
     *
     * Classes is implemented as an alias to a cpp::meta::list<> instance with the member enumeration types of the class
     */
    using Enums = Enums_;
};

}

namespace codegen
{
    template<typename T>
    class Class :
        public static_reflection::meta::Class<
            static_reflection::meta::EmptySourceInfo<T>,
            T,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>
        >
    {};
}

/**
 * \ingroup static-reflection
 * \brief Returns information of a given class
 *
 * This template returns (inherits) a cpp::static_reflection::meta::Class instance with the information
 * of the given class type. If there's no static reflection information of this class in the current translation
 * unit, returns an empty cpp::static_reflection::meta::Class instance (Default "unknown" source info, empty methods, ctors,
 * enums, and classes lists, etc). The behavior is undefined if the given type is not a class type.
 *
 * See cpp::static_reflection::meta::Class for the returned information.
 *
 *
 * ``` cpp
 *
 * template<typename T, typename = std::enable_if_t<!std::is_basic_type<T>::value>>
 * std::string serialize(const T& object)
 * {
 *     std::ostringstream os;
 *
 *     os << cpp::static_reflection::Class<T>::fullName() << ": {";
 *
 *     cpp::foreach_type<typename cpp::static_reflection::Class<T>::Fields>([&](auto type)
 *     {
 *         using FieldInfo = cpp::meta::type_t<decltype(type)>;
 *
 *         os << serialize(object.*FieldInfo::get()) << ", ";
 *     });
 *
 *     os << "}";
 *
 *     return os.str();
 * }
 *
 * template<typename T, std::enable_if_t<std::is_basic_type<T>::value>>
 * std::string serialize(T value)
 * {
 *     std::ostream os;
 *     os << ctti::type_id<T>().name() << ": " << value;
 *     return os.str();
 * }
 *
 * template<typename T>
 * std::string serialize(const std::vector<T>& values)
 * {
 *     std::ostream os;
 *
 *     os << "[";
 *
 *     for(const T& value : values)
 *     {
 *         os << serialize(value) << ", ";
 *     }
 *
 *     os << "]";
 *
 *     return os.str();
 * }
 *
 * std::string serialize(const std::string& value)
 * {
 *     return value;
 * }
 *
 * ```
 *
 * \tparam T Must be a class type.
 */
template<typename T>
class Class : public codegen::Class<T>
{
    using type = T;
};

}

}

#endif // SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP
