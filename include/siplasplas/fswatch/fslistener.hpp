#ifndef SIPLASPLAS_SIPLASPLAS_FSWATCH_FSLISTENER_HPP
#define SIPLASPLAS_SIPLASPLAS_FSWATCH_FSLISTENER_HPP

#include <efsw/efsw.hpp>
#include <siplasplas/signals/emitter.hpp>

namespace cpp
{

class FileSystemListener : public efsw::FileWatchListener, public cpp::SignalEmitter
{
public:
    void fileCreated(efsw::WatchID watchId, const std::string& dir, const std::string& fileName) {}
    void fileDeleted(efsw::WatchID watchId, const std::string& dir, const std::string& fileName) {}
    void fileModified(efsw::WatchID watchId, const std::string& dir, const std::string& fileName) {}
    void fileMoved(efsw::WatchID watchId, const std::string& dir, const std::string& oldFilename, const std::string& newFileName) {}

private:
    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename);
};

}
#include <reflection/include/siplasplas/fswatch/fslistener.hpp>

#endif // SIPLASPLAS_SIPLASPLAS_FSWATCH_FSLISTENER_HPP
