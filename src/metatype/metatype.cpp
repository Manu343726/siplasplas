#include "metatype.hpp"

#include <sstream>
#include <siplasplas/utility/throw.hpp>

using namespace cpp;

MetaTypeSystem::MetaTypeRegistry MetaTypeSystem::registry;

void* MetaTypeSystem::create(const std::string& typeName)
{
	// MSVC mangling adds struct/class qualifiers to type names
    std::string className = "class " + typeName;
    std::string structName = "struct " + typeName;

    void* result = nullptr;

    if (!(result = try_create(typeName)))
    {
        if (!(result = try_create(className)))
        {
            if (!(result = try_create(structName)))
            {
    cpp::Throw<std::runtime_error>()
	<< "[METATYPE_SYSTEM] Type '" << typeName << "' not registered!\n"
	   " - As plain type (" << typeName << "): Hash " << ctti::id_from_name(typeName).hash() << "\n"
       " - As class type (" << className << "): Hash " << ctti::id_from_name(className).hash() <<  "\n"
       " - As struct type (" << structName << "): Hash " << ctti::id_from_name(structName).hash() + "\n"
    << dump();
            }
        }
    }

    return result;
}

std::string MetaTypeSystem::dump()
{
    std::ostringstream os;

    os << "[METATYPE_SYSTEM] REGISTRY DUMP:" << std::endl;

    for(const auto& keyValue : registry)
    {
        const auto& id = keyValue.first;

        os << " - Hash: " << id.hash() << std::endl;
    }

    return os.str();
}

void* MetaTypeSystem::try_create(const std::string& typeName)
{
    auto id = ctti::id_from_name(typeName);

    auto it = registry.find(id);

    if (it != registry.end())
    {
        return it->second->create();
    }
    else
    {
        return nullptr;
    }
}