#ifndef SIPLASPLAS_SERIALIZATION_SERIALIZATION_HPP
#define SIPLASPLAS_SERIALIZATION_SERIALIZATION_HPP

#include <siplasplas/reflection/api.hpp>
#include <siplasplas/serialization/json.hpp>

namespace cpp
{

cpp::json serialize(const cpp::MetaObject& object);
cpp::MetaObject deserialize(const cpp::json& json);

template<typename T>
cpp::json serialize(const T& object)
{
    return serialize(cpp::MetaObject{object});
}

template<typename T>
T deserialize(const cpp::json& json)
{
    return deserialize(json).get<T>();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector)
{
    return os << cpp::json(vector);
}



}

#endif // SIPLASPLAS_SERIALIZATION_SERIALIZATION_HPP
