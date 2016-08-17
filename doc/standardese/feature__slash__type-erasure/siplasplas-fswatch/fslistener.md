---
layout: standardese-doc
---

# Header file `fslistener.hpp`

``` cpp
#define SIPLASPLAS_SIPLASPLAS_FSWATCH_FSLISTENER_HPP 

#include <efsw/efsw.hpp>

#include <siplasplas/signals/emitter.hpp>

#include <siplasplas/fswatch/export.hpp>

#include <reflection/include/siplasplas/fswatch/fslistener.hpp>

namespace cpp
{
    class FileSystemListener;
}
```

## Class `cpp::FileSystemListener`<a id="cpp::FileSystemListener"></a>

``` cpp
class FileSystemListener
: public cpp::SignalEmitter
{
public:
    void fileCreated(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
    
    void fileDeleted(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
    
    void fileModified(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
    
    void fileMoved(efsw::WatchID watchId, const std::string& dir, const std::string& oldFilename, const std::string& newFileName);
};
```

File system listener class that notifies filesystem changed through signals

FileSystemListener is a wrapper of an [Euforia Filesystem Watcher]() filesystem listener that uses siplasplas signals to notify changes on the file system, using a different signal for each change type (File created, file modified, etc). The class implements the efsw::FileWatchListener interface, and can be passed as listener for efsw watchers. The signals are raised from the background thread the filesystem watcher is running.

### Function `cpp::FileSystemListener::fileCreated`<a id="cpp::FileSystemListener::fileCreated"></a>

``` cpp
void fileCreated(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
```

Signal emitted whenever a file/directory is created.

#### Parameter `cpp::FileSystemListener::fileCreated::watchId`<a id="cpp::FileSystemListener::fileCreated::watchId"></a>

``` cpp
efsw::WatchID watchId
```

ID of the watch the event comes from. \\param dir base directory of the entity (file or directory) that raised the event. \\param fileName Filename of the entity that raised the event.

-----

### Function `cpp::FileSystemListener::fileDeleted`<a id="cpp::FileSystemListener::fileDeleted"></a>

``` cpp
void fileDeleted(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
```

Signal emitted whenever a file/directory is deleted.

#### Parameter `cpp::FileSystemListener::fileDeleted::watchId`<a id="cpp::FileSystemListener::fileDeleted::watchId"></a>

``` cpp
efsw::WatchID watchId
```

ID of the watch the event comes from. \\param dir base directory of the entity (file or directory) that raised the event. \\param fileName Filename of the entity that raised the event.

-----

### Function `cpp::FileSystemListener::fileModified`<a id="cpp::FileSystemListener::fileModified"></a>

``` cpp
void fileModified(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
```

Signal emitted whenever a file/directory is modified.

#### Parameter `cpp::FileSystemListener::fileModified::watchId`<a id="cpp::FileSystemListener::fileModified::watchId"></a>

``` cpp
efsw::WatchID watchId
```

ID of the watch the event comes from. \\param dir base directory of the entity (file or directory) that raised the event. \\param fileName Filename of the entity that raised the event.

-----

### Function `cpp::FileSystemListener::fileMoved`<a id="cpp::FileSystemListener::fileMoved"></a>

``` cpp
void fileMoved(efsw::WatchID watchId, const std::string& dir, const std::string& oldFilename, const std::string& newFileName);
```

Signal emitted whenever a file/directory is moved.

#### Parameter `cpp::FileSystemListener::fileMoved::watchId`<a id="cpp::FileSystemListener::fileMoved::watchId"></a>

``` cpp
efsw::WatchID watchId
```

ID of the watch the event comes from. \\param dir base directory of the entity (file or directory) that raised the event. \\param oldFIlename Old name of the directory/file. \\param newFIlename New name of the directory/file.

-----

-----
