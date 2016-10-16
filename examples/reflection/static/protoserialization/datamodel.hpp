#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_DATAMODEL_HPP
#define SIPLAPSLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_DATAMODEL_HPP

#include <string>
#include <chrono>

class NetworkSettings
{
public:
    std::string ipAddress = "";
    std::string gateway = "";
    std::chrono::milliseconds pingInterval = std::chrono::milliseconds(0);
};

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_DATAMODEL_HPP
