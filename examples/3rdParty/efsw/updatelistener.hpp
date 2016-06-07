#ifndef SIPLASPLAS_EXAMPLES_EFSW_UPDATELISTENER_HPP
#define SIPLASPLAS_EXAMPLES_EFSW_UPDATELISTENER_HPP

#include <efsw/efsw.hpp>
#include <siplasplas/signals/emitter.hpp>

// Inherits from the abstract listener class, and implements the the file action handler
class UpdateListener : public efsw::FileWatchListener, public cpp::SignalEmitter
{
public:
    UpdateListener() {}

    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename);
    void signalFileAction( efsw::WatchID watchid, std::string dir, std::string filename, efsw::Action action, std::string oldFilename){}
};

#include <reflection/examples/3rdParty/efsw/updatelistener.hpp>

void UpdateListener::handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename)
{
    cpp::emit(*this).signalFileAction(watchid, dir, filename, action, oldFilename);
}


#endif // SIPLASPLAS_EXAMPLES_EFSW_UPDATELISTENER_HPP
