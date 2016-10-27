#include "datamodel.hpp"
#include "mapping.hpp"
#include <commmodel.pb.h>
#include <ctti/type_id.hpp>
#include <reflection/examples/reflection/static/protoserialization/datamodel.hpp>
#include <reflection/build/examples/reflection/static/protoserialization/commmodel.pb.h>
#include <iostream>

int main()
{
    // Fill some protos:
    commmodel::NetworkSettings networkSettingsProto;
    commmodel::ServerSettings serverSettingsProto;
    commmodel::Settings settingsProto;
    commmodel::SettingsOperation settingsOperationProto;

    networkSettingsProto.set_ipaddress("192.168.1.2");
    networkSettingsProto.set_gateway("192.168.1.1");
    networkSettingsProto.set_pingintervalms(10000);
    serverSettingsProto.set_port(4242);
    *settingsProto.mutable_networksettings() = networkSettingsProto;
    *settingsProto.mutable_serversettings()  = serverSettingsProto;
    settingsOperationProto.set_operation(commmodel::Operation::Set);
    *settingsOperationProto.mutable_settings() = settingsProto;

    // Translate them to the data model:
    const NetworkSettings networkSettingsData{
        mapping::read<NetworkSettings>(networkSettingsProto)
    };
    const ServerSettings serverSettingsData{
        mapping::read<ServerSettings>(serverSettingsProto)
    };
    const Settings settingsData{
        mapping::read<Settings>(settingsProto)
    };
    const SettingsOperation settingsOperationData{
        mapping::read<SettingsOperation>(settingsOperationProto)
    };

    // Translate the data model back to protos:
    const commmodel::NetworkSettings networkSettingsProtoAgain{
        mapping::write<commmodel::NetworkSettings>(networkSettingsData)
    };
    const commmodel::ServerSettings serverSettingsProtoAgain{
        mapping::write<commmodel::ServerSettings>(serverSettingsData)
    };
    const commmodel::Settings settingsProtoAgain{
        mapping::write<commmodel::Settings>(settingsData)
    };
    const commmodel::SettingsOperation settingsOperationProtoAgain{
        mapping::write<commmodel::SettingsOperation>(settingsOperationData)
    };


    std::cout << "C++ data model: " << networkSettingsData << std::endl;
    std::cout << "C++ data model: " << serverSettingsData << std::endl;
    std::cout << "C++ data model: " << settingsData << std::endl;
    std::cout << "C++ data model: " << settingsOperationData << std::endl;
    std::cout << "Protobuf comm model: " << networkSettingsProtoAgain.DebugString() << std::endl;
    std::cout << "Protobuf comm model: " << serverSettingsProtoAgain.DebugString() << std::endl;
    std::cout << "Protobuf comm model: " << settingsProtoAgain.DebugString() << std::endl;
    std::cout << "Protobuf comm model: " << settingsOperationProtoAgain.DebugString() << std::endl;
}
