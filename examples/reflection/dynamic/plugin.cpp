#include <siplasplas/reflection/dynamic/runtimeloader.hpp>
#include <siplasplas/reflection/dynamic/enum.hpp>
#include <iostream>

using namespace cpp;
using namespace cpp::dynamic_reflection;

/*
 * This example shows how the dynamic reflection API
 * can be used to load plugin lybraries at runtime
 *
 * First the plugin library exposes its API types as follows:
 *
 *     SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT_TYPES(
 *         Plugin
 *     )
 *
 * (See examples/reflectiondynamic/plugin/export.cpp)
 *
 * The built plugin library is loaded at runtime using
 * cpp::DynamicLibrary API and then passed to the dynamic
 * reflection API which loads a runtime from the plugin
 *
 * All types exported by the plugin library are available in
 * the loaded runtime.
 */

int main()
{
    // Load the plugin library and a dynamic reflection runtime
    // from it:
    auto pluginLibrary = DynamicLibrary::load(PLUGIN_LIBRARY);
    RuntimeLoader loader{pluginLibrary};
    Runtime& runtime = loader.runtime();

    // Extract types from the plugin library:
    Class& pluginClass = runtime.namespace_().class_("Plugin");
    Enum& operationEnum = pluginClass.class_("ALU").enum_("Operation");

    // Create objects of types imported from the plugin library
    //
    // Objects are represented as cpp::Any32 objects. For class
    // types, the returned any has all the class member functions and
    // objects assigned, giving a OOP Javascript-like way to manipulate
    // plugin API objects:
    cpp::Any32 plugin = pluginClass.create();
    cpp::Any32 alu = pluginClass.class_("ALU").create();

    plugin("run")();
    std::cout << "Plugin::ALU::add(20, 22): " << alu("add")(20, 22).get<int>() << std::endl;

    for(std::size_t i = 0; i < operationEnum.count(); ++i)
    {
        std::cout << operationEnum.fullName() << "::" << operationEnum.name(i) << ": " << operationEnum.value(i) << std::endl;
    }
}
