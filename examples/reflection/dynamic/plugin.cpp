#include <plugininterface/plugininterface.hpp>
#include <siplasplas/reflection/dynamic/runtimeloader.hpp>
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

    Object pluginInstance{pluginClass.type()};
    PluginInterface& plugin = pluginInstance.get<PluginInterface>();
    plugin.run();
}

