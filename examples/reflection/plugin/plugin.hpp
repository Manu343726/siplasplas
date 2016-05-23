#ifndef SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP
#define SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP

#include <plugininterface/plugininterface.hpp>

class Plugin : public PluginInterface
{
public:
    void run() override;
};

#include <reflection/examples/reflection/plugin/plugin.hpp>

#endif // SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP
