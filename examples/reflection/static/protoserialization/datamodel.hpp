#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_DATAMODEL_HPP
#define SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_DATAMODEL_HPP

#include <string>
#include <chrono>
#include <ostream>

class NetworkSettings
{
public:
    std::string ipAddress = "";
    std::string gateway = "";
    std::chrono::milliseconds pingInterval = std::chrono::milliseconds(0);
};

class ServerSettings
{
public:
    int port = 8080;
};

class Settings
{
public:
    NetworkSettings netwokSettings;
    ServerSettings serverSettings;
};

enum class Operation
{
    Set,
    Get
};

class SettingsOperation
{
public:
    Operation operation;
    Settings settings;
};

class MessageWithOperation
{
public:
    Operation operation;
};

#include <reflection/examples/reflection/static/protoserialization/datamodel.hpp>

std::ostream& operator<<(std::ostream& os, const NetworkSettings& networkSettings)
{
    return os << "NetworkSettings {\n"
              << "  ipAddress: " << networkSettings.ipAddress
              << "\n  gateway: " << networkSettings.gateway
              << "\n  pingInterval: " << networkSettings.pingInterval.count() << " ms"
              << "\n}";
}

std::ostream& operator<<(std::ostream& os, const ServerSettings& serverSettings)
{
    return os << "ServerSettings {\n"
              << "  port: " << serverSettings.port
              << "\n}";
}

std::ostream& operator<<(std::ostream& os, const Settings& settings)
{
    return os << "Settings{\n"
              << "  networkSettings: " << settings.netwokSettings
              << "\n  serverSettings: " << settings.serverSettings
              << "\n}";
}

std::ostream& operator<<(std::ostream& os, const Operation& operation)
{
    return os << "Operation::" << cpp::static_reflection::Enum<Operation>::toString(operation);
}

std::ostream& operator<<(std::ostream& os, const SettingsOperation& settingsOperation)
{
    return os << "SettingsOperation{\n"
              << "  operation: " << settingsOperation.operation
              << "\n  settings: " << settingsOperation.settings
              << "\n}";
}

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_DATAMODEL_HPP
