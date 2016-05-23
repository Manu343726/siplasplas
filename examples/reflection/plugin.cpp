#include <plugininterface/plugininterface.hpp>
#include <siplasplas/reflection/dynamic/runtimeloader.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;

int main()
{
    auto pluginLibrary = DynamicLibrary::load("plugin/libpluginexample.so");
    RuntimeLoader loader{pluginLibrary};
    Runtime& runtime = loader.runtime();
    Class& pluginClass = runtime.namespace_().class_("Plugin");

    Object pluginInstance{pluginClass.type()};
    PluginInterface& plugin = pluginInstance.get<PluginInterface>();
    plugin.run();
}

