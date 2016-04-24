#include "serialization.hpp"

#include <siplasplas/reflection/api.hpp>

namespace cpp
{

cpp::json serialize(const cpp::MetaObject& object)
{
    const auto& class_ = cpp::reflection(object.type());
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

cpp::MetaObject deserialize(const cpp::json& json)
{
    const auto& type = cpp::MetaType::get(json["type"].get<std::string>());
    auto& class_ = cpp::reflection(type);

    if(class_.fields().empty())
    {
        const std::string& type = json["type"];
        const std::string& value = json["value"];

        return cpp::MetaObject::fromString(type, value);
    }
    else
    {
        cpp::MetaObject object{type};

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
