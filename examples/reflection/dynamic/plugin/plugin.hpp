#ifndef SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP
#define SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP

#include <plugininterface/plugininterface.hpp>
#include <examples/reflection/dynamic/plugin/export.hpp>

class PLUGINEXAMPLE_EXPORT Plugin : public PluginInterface
{
public:
    void run() override;

    class ALU
    {
    public:
        enum class Operation
        {
            ADD, SUB
        };

        int add(int a, int b);
        int sub(int a, int b);
    };
};

#include <reflection/examples/reflection/dynamic/plugin/plugin.hpp>

#endif // SIPLASPLAS_EXAMPLES_PLUGIN_PLUGIN_HPP
