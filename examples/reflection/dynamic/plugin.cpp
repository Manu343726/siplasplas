#include <plugininterface/plugininterface.hpp>
#include <siplasplas/reflection/dynamic/runtimeloader.hpp>
#include <siplasplas/reflection/dynamic/object.hpp>
#include <iostream>

using namespace cpp;
using namespace cpp::dynamic_reflection;

int main()
{
    auto pluginLibrary = DynamicLibrary::load(PLUGIN_LIBRARY);
    RuntimeLoader loader{pluginLibrary};
    Runtime& runtime = loader.runtime();
    Class& pluginClass = runtime.namespace_().class_("Plugin");

    std::cout << "class '" << pluginClass.fullName() << "' loaded from shared library " << PLUGIN_LIBRARY << std::endl;

    cpp::Any32 plugin = pluginClass.create();
    cpp::Any32 alu = pluginClass.class_("ALU").create();

    // Invoke Plugin::run():
    plugin("run")();

    std::cout << "Plugin::ALU::add(20, 22): " << alu("add")(20, 22).get<int>() << std::endl;
}
