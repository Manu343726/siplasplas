#include "serialization.hpp"

using cpp::dynamic_reflection::Type;
using cpp::dynamic_reflection::Class;
using cpp::dynamic_reflection::Object;

namespace cpp
{

cpp::json serialize(const Object& object)
{
    const auto& class_ = Class::get(object.type());
    cpp::json jsonObject;

    jsonObject["type"] = object.type().typeName();

    if(class_.fields().empty())
    {
        jsonObject["value"] = object.toString();
    }
    else
    {
        jsonObject["fields"] = cpp::json::object();

        for(const auto& keyValue : class_.fields())
        {
            const auto& name = keyValue.first;
            const auto& field = keyValue.second;

            jsonObject["fields"][name] = serialize(field.get(object));
        }
    }

    return jsonObject;
}

Object deserialize(const cpp::json& json)
{
    const auto& type = Type::get(json["type"].get<std::string>());
    auto& class_     = Class::get(type);

    if(class_.fields().empty())
    {
        const std::string& type = json["type"];
        const std::string& value = json["value"];

        return Object::fromString(type, value);
    }
    else
    {
        Object object{type};

        for(auto& keyValue : class_.fields())
        {
            const auto& name = keyValue.first;
            auto& field = keyValue.second;

            field.get(object) = deserialize(json["fields"][name]);
        }

        return object;
    }
}

}
