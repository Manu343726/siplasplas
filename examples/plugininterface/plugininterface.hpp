#ifndef SIPLASPLAS_EXAMPLES_PLUGININTERFACE_HPP
#define SIPLASPLAS_EXAMPLES_PLUGININTERFACE_HPP

class PluginInterface
{
public:
    virtual ~PluginInterface() = default;

    virtual void run() = 0;
};

#endif // SIPLASPLAS_EXAMPLES_PLUGININTERFACE_HPP
