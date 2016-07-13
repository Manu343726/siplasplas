#ifndef SIPLASPLAS_SIPLASPLAS_FSWATCH_FSLISTENER_HPP
#define SIPLASPLAS_SIPLASPLAS_FSWATCH_FSLISTENER_HPP

#include <efsw/efsw.hpp>
#include <siplasplas/signals/emitter.hpp>

/**
 * \defgroup fswatch
 *
 * \brief An [Euforia Filesystem Watcher](https://bitbucket.org/SpartanJ/efsw) wrapper that uses signals to notify filesystem
 * changes.
 *
 * The efsw API works by instancing an efsw::FileSystemWatcher that wraps OS specific watching mechanisms
 * running on a background thread. The user registers different *watches* that could look for changes in
 * different directories using the same background watcher. A watch is connected to a efsw::FileWatchListener
 * that is notified whenever an event occurs. Since both watchers and listeners can be shared between multiple
 * whatches, each watch has an associated unique watch ID. That IDs can be used to identify what watch (i.e. what
 * directory) the event comes from.
 *
 * See cpp::FileSystemListener
 *
 * \example fswatch/fswatch.cpp
 */

namespace cpp
{

/**
 * \ingroup fswatch
 *
 *
 * \brief File system listener class that notifies filesystem changed through signals
 *
 * FileSystemListener is a wrapper of an [Euforia Filesystem Watcher]() filesystem listener
 * that uses siplasplas signals to notify changes on the file system, using a different signal for
 * each change type (File created, file modified, etc).
 * The class implements the efsw::FileWatchListener interface, and can be passed as listener for
 * efsw watchers. The signals are raised from the background thread the filesystem watcher is running.
 */
class FileSystemListener : public efsw::FileWatchListener, public cpp::SignalEmitter
{
public:
    /**
     * \brief Signal emitted whenever a file/directory is created.
     *
     * \param watchId ID of the watch the event comes from.
     * \param dir base directory of the entity (file or directory) that raised the event.
     * \param fileName Filename of the entity that raised the event.
     */
    void fileCreated(efsw::WatchID watchId, const std::string& dir, const std::string& fileName) {}

    /**
     * \brief Signal emitted whenever a file/directory is deleted.
     *
     * \param watchId ID of the watch the event comes from.
     * \param dir base directory of the entity (file or directory) that raised the event.
     * \param fileName Filename of the entity that raised the event.
     */
    void fileDeleted(efsw::WatchID watchId, const std::string& dir, const std::string& fileName) {}

    /**
     * \brief Signal emitted whenever a file/directory is modified.
     *
     * \param watchId ID of the watch the event comes from.
     * \param dir base directory of the entity (file or directory) that raised the event.
     * \param fileName Filename of the entity that raised the event.
     */
    void fileModified(efsw::WatchID watchId, const std::string& dir, const std::string& fileName) {}

    /**
     * \brief Signal emitted whenever a file/directory is moved.
     *
     * \param watchId ID of the watch the event comes from.
     * \param dir base directory of the entity (file or directory) that raised the event.
     * \param oldFIlename Old name of the directory/file.
     * \param newFIlename New name of the directory/file.
     */
    void fileMoved(efsw::WatchID watchId, const std::string& dir, const std::string& oldFilename, const std::string& newFileName) {}

private:
    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename);
};

}
#include <reflection/include/siplasplas/fswatch/fslistener.hpp>

#endif // SIPLASPLAS_SIPLASPLAS_FSWATCH_FSLISTENER_HPP
