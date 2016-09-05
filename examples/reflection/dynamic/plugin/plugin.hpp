#ifndef SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP
#define SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP

#include <plugininterface/plugininterface.hpp>
#include <examples/reflection/plugin/export.hpp>

class PLUGINEXAMPLE_EXPORT Plugin : public PluginInterface
{
public:
    void run() override;
};

#include <reflection/examples/reflection/plugin/plugin.hpp>

#endif // SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP
