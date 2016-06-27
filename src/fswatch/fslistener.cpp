#include "fslistener.hpp"

using namespace cpp;

void FileSystemListener::handleFileAction(efsw::WatchID watchId, const std::string& dir, const std::string& fileName, efsw::Action action, std::string oldFileName)
{
    switch(action)
    {
    case efsw::Actions::Add:
        cpp::emit(*this).fileCreated(watchId, dir, fileName); break;
    case efsw::Actions::Delete:
        cpp::emit(*this).fileDeleted(watchId, dir, fileName); break;
    case efsw::Actions::Modified:
        cpp::emit(*this).fileModified(watchId, dir, fileName); break;
    case efsw::Actions::Moved:
        cpp::emit(*this).fileMoved(watchId, dir, oldFileName, fileName); break;
    }
}
