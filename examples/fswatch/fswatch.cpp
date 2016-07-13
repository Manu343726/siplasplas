#include <siplasplas/fswatch/fslistener.hpp>
#include <iostream>

int main()
{
    efsw::FileWatcher watcher;
    cpp::FileSystemListener listener;

    cpp::SignalEmitter::connect(listener, &cpp::FileSystemListener::fileCreated, [&](efsw::WatchID watchId, const std::string& directory, const std::string& filename)
    {
        std::cout << fmt::format("(watch {}) {}, {}: file created", watchId, directory, filename) << std::endl;
    });
    cpp::SignalEmitter::connect(listener, &cpp::FileSystemListener::fileDeleted, [&](efsw::WatchID watchId, const std::string& directory, const std::string& filename)
    {
        std::cout << fmt::format("(watch {}) {}, {}: file deleted", watchId, directory, filename) << std::endl;
    });
    cpp::SignalEmitter::connect(listener, &cpp::FileSystemListener::fileModified, [&](efsw::WatchID watchId, const std::string& directory, const std::string& filename)
    {
        std::cout << fmt::format("(watch {}) {}, {}: file modified", watchId, directory, filename) << std::endl;
    });
    cpp::SignalEmitter::connect(listener, &cpp::FileSystemListener::fileMoved, [&](efsw::WatchID watchId, const std::string& directory, const std::string& oldFilename, const std::string& newFilename)
    {
        std::cout << fmt::format("(watch {}) {}, (oldfile {}) {} : file moved", watchId, directory, oldFilename, newFilename) << std::endl;
    });

    // Recursively watch the /tmp directory
    efsw::WatchID watchId = watcher.addWatch("/tmp", &listener, true);
    watcher.watch();

    while(true); // Watch until you completely drain your laptop battery...
}
