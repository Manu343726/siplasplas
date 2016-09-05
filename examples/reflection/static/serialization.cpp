
#include "../myclass.hpp"
#include <ctti/type_id.hpp>
#include <siplasplas/utility/fusion.hpp>
#include <siplasplas/utility/invoke.hpp>
#include <json4moderncpp/src/json.hpp>
#include <iostream>
#include <sstream>

template<typename T>
std::enable_if_t<std::is_fundamental<T>::value, nlohmann::json>
serialize(const T& object);
template<typename T>
std::enable_if_t<std::is_class<T>::value, nlohmann::json>
serialize(const T& object);
template<typename T>
nlohmann::json serialize(const std::vector<T>& values);
nlohmann::json serialize(const std::string& value);
template<typename... Ts>
nlohmann::json serialize(const std::tuple<Ts...>& value);
template<typename First, typename Second>
nlohmann::json serialize(const std::pair<First, Second>& value);
template<typename Key, typename Value>
nlohmann::json serialize(const std::unordered_map<Key, Value>& value);




template<typename T>
std::enable_if_t<std::is_fundamental<T>::value, nlohmann::json>
serialize(const T& value)
{
    auto json = nlohmann::json::object();

    json["type"]  = cpp::lexical_cast(ctti::type_id(value).name());
    json["value"] = value;

    return json;
}

template<typename T>
nlohmann::json serialize(const std::vector<T>& values)
{
    auto json  = nlohmann::json::object();
    auto array = nlohmann::json::array();

    for(const T& value : values)
    {
        array.push_back(serialize(value));
    }

    json["type"]  = cpp::lexical_cast(ctti::type_id(values).name());
    json["value"] = array;

    return json;
}

nlohmann::json serialize(const std::string& value)
{
    auto json = nlohmann::json::object();

    json["type"]  = cpp::lexical_cast(ctti::type_id(value).name());
    json["value"] = value;

    return json;
}

template<typename... Ts>
nlohmann::json serialize(const std::tuple<Ts...>& value)
{
    auto json  = nlohmann::json::object();
    auto array = nlohmann::json::array();

    cpp::foreach_type<cpp::meta::make_index_sequence_for<Ts...>>([&](auto type)
    {
        using Index = cpp::meta::type_t<decltype(type)>;

        array.push_back(serialize(std::get<Index::value>(value)));
    });

    json["type"]  = cpp::lexical_cast(ctti::type_id(value).name());
    json["value"] = array;

    return json;
}

template<typename First, typename Second>
nlohmann::json serialize(const std::pair<First, Second>& value)
{
    auto json  = nlohmann::json::object();
    auto array = nlohmann::json::array();

    array.push_back(serialize(value.first));
    array.push_back(serialize(value.second));

    json["type"]  = cpp::lexical_cast(ctti::type_id(value).name());
    json["value"] = array;

    return json;
}

template<typename Key, typename Value>
nlohmann::json serialize(const std::unordered_map<Key, Value>& value)
{
    auto json   = nlohmann::json::object();
    auto array  = nlohmann::json::array();

    for(const auto& keyValue : value)
    {
        const auto& key   = keyValue.first;
        const auto& value = keyValue.second;

        array.push_back(nlohmann::json::object({
            {"key", serialize(key)},
            {"value", serialize(value)}
        }));
    }

    json["type"]  = cpp::lexical_cast(ctti::type_id(value).name());
    json["value"] = array;

    return json;
}

template<typename First, typename Second>
nlohmann::json serialize(const std::pair<First, Second>& value);

template<typename Key, typename Value>
nlohmann::json serialize(const std::unordered_map<Key, Value>& value);

template<typename T>
std::enable_if_t<std::is_class<T>::value, nlohmann::json> serialize(const T& object)
{
    auto json = nlohmann::json::object();
    auto fields = nlohmann::json::object();

    cpp::foreach_type<typename cpp::static_reflection::Class<T>::Fields>([&](auto type)
    {
        using FieldInfo = cpp::meta::type_t<decltype(type)>;

        fields[FieldInfo::SourceInfo::spelling()] = serialize(
            cpp::invoke(FieldInfo::get(), object) // C++17 std::invoke() on member object ptr
        );
    });

    json["type"]  = cpp::lexical_cast(ctti::type_id(object).name());
    json["value"] = fields;

    return json;
}

namespace
{

template<typename T, typename = void>
class Deserialize;

template<typename T>
class Deserialize<T, std::enable_if_t<std::is_fundamental<T>::value>>
{
public:
    static T apply(const nlohmann::json& json)
    {
        SIPLASPLAS_ASSERT_EQ(json["type"].get<std::string>(), cpp::lexical_cast(ctti::type_id<T>().name()));

        return json["value"];
    }
};

template<typename T>
class Deserialize<T, std::enable_if_t<std::is_class<T>::value>>
{
public:
    static T apply(const nlohmann::json& json)
    {
        SIPLASPLAS_ASSERT_EQ(json["type"].get<std::string>(), cpp::lexical_cast(ctti::type_id<T>().name()));

        const auto& fields = json["value"];
        T object;

        cpp::foreach_type<typename cpp::static_reflection::Class<T>::Fields>([&](auto type)
        {
            using FieldInfo = cpp::meta::type_t<decltype(type)>;
            using Type = typename FieldInfo::value_type;

            cpp::invoke(FieldInfo::get(), object) = Deserialize<Type>::apply(fields[FieldInfo::SourceInfo::spelling()]);
        });

        return object;
    }
};

template<typename T>
class Deserialize<std::vector<T>, void>
{
public:
    static std::vector<T> apply(const nlohmann::json& json)
    {
        SIPLASPLAS_ASSERT(json["value"].is_array());
        SIPLASPLAS_ASSERT_EQ(json["type"].get<std::string>(), cpp::lexical_cast(ctti::type_id<std::vector<T>>().name()));

        std::vector<T> vector;
        vector.reserve(json.size());

        for(const auto& value : json)
        {
            vector.emplace_back(value);
        }

        return vector;
    }
};

template<>
class Deserialize<std::string, void>
{
public:
    static std::string apply(const nlohmann::json& json)
    {
        SIPLASPLAS_ASSERT(json["value"].is_string());
        SIPLASPLAS_ASSERT_EQ(json["type"].get<std::string>(), cpp::lexical_cast(ctti::type_id<std::string>().name()));

        return json["value"];
    }
};

template<typename Key, typename Value>
class Deserialize<std::unordered_map<Key, Value>, void>
{
public:
    static std::unordered_map<Key, Value> apply(const nlohmann::json& json)
    {
        SIPLASPLAS_ASSERT(json["value"].is_object());
        SIPLASPLAS_ASSERT_EQ(json["type"].get<std::string>(), cpp::lexical_cast(ctti::type_id<std::unordered_map<Key, Value>>().name()));

        std::unordered_map<Key, Value> map;

        for(const auto& keyValue : json)
        {
            map[Deserialize<Key>::apply(keyValue["jey"])] = Deserialize<Value>::apply(keyValue["value"]);
        }

        return map;
    }
};

template<typename... Ts>
class Deserialize<std::tuple<Ts...>, void>
{
public:
    static std::tuple<Ts...> apply(const nlohmann::json& json)
    {
        SIPLASPLAS_ASSERT(json["value"].is_array());
        SIPLASPLAS_ASSERT_EQ(json["type"].get<std::string>(), cpp::lexical_cast(ctti::type_id<std::tuple<Ts...>>().name()));

        std::tuple<Ts...> tuple;

        cpp::foreach_type<cpp::meta::make_index_sequence_for<Ts...>>([&](auto type)
        {
            using Index = cpp::meta::type_t<decltype(type)>;

            std::get<Index::value>(tuple) = Deserialize<cpp::meta::pack_get_t<Index::value, Ts...>>::apply(json["value"][Index::value]);
        });

        return tuple;
    }
};

template<typename First, typename Second>
class Deserialize<std::pair<First, Second>, void>
{
public:
    static std::pair<First, Second> apply(const nlohmann::json& json)
    {
        SIPLASPLAS_ASSERT(json["value"].is_object());
        SIPLASPLAS_ASSERT_EQ(json["type"].get<std::string>(), cpp::lexical_cast(ctti::type_id<std::pair<First, Second>>().name()));

        return std::make_pair(
            Deserialize<First>::apply(json["value"]["first"]),
            Deserialize<Second>::apply(json["value"]["second"])
        );
    }
};

}

template<typename T>
T deserialize(const nlohmann::json& json)
{
    return Deserialize<T>::apply(json);
}

int main()
{
    MyClass myObject;
    auto json = serialize(myObject);
    auto deserializedObject = deserialize<MyClass>(json);

    std::cout << "Serialized object: " << json.dump(1) << std::endl;
    std::cout << "Re-serialized deserialized object: " << serialize(deserializedObject).dump(1) << std::endl;
}
