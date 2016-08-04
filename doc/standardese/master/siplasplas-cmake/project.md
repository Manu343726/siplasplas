# Header file `project.hpp`

``` cpp
#define SIPLASPLAS_CMAKE_PROJECT_HPP 

#include <string>

#include <siplasplas/signals/emitter.hpp>

#include <siplasplas/fswatch/fslistener.hpp>

#include "target.hpp"

#include <reflection/include/siplasplas/cmake/project.hpp>

namespace cpp
{
    class CMakeProject;
}
```

## Class `cpp::CMakeProject`<a id="cpp::CMakeProject"></a>

``` cpp
class CMakeProject
: public cpp::SignalEmitter
{
public:
    CMakeProject(const std::string& sourceDir, const std::string& binaryDir);
    
    void configure();
    
    void buildTarget(const std::string& targetName);
    
    CMakeTarget& addTarget(const std::string& name);
    
    CMakeTarget& addTarget(const CMakeTarget::Metadata& metadata);
    
    efsw::WatchID addSourceDirWatch();
    
    efsw::WatchID addIncludeDirWatch();
    
    efsw::WatchID addBinaryDirWatch();
    
    void startWatch();
    
    void configureFinished(bool successful);
    
    void buildStarted(const std::string& target);
    
    void buildFinished(const std::string& target, bool successful);
    
    void stdoutLine(const std::string& targetName, const std::string& line);
    
    cpp::FileSystemListener& fileSystemListener();
    
    const std::string& sourceDir() const;
    
    const std::string& binaryDir() const;
};
```

Configures and builds an existing CMake project.

CMakeProject class handles an existing CMake project given its source and build directories. The project can be configured and built manually (See CMakeProject::configure() and CMakeProject::buildTarget() functions) or targets can be registered to be watched and rebuilt on source changes.

The class notifies project events through signals, such as when the configuration finished, a target build started, etc.

### Constructor `cpp::CMakeProject::CMakeProject`<a id="cpp::CMakeProject::CMakeProject"></a>

``` cpp
CMakeProject(const std::string& sourceDir, const std::string& binaryDir);
```

Creates a cmake project given its source and build directories

Source and build directories are inmutable and can only be set during construction. Once the cmake project is initialized, target watching is not launched by default and no targets are registered.

#### Parameter `cpp::CMakeProject::CMakeProject::sourceDir`<a id="cpp::CMakeProject::CMakeProject::sourceDir"></a>

``` cpp
const std::string& sourceDir
```

Path to the project source directory \\param binaryDir Path to the project binary directory

-----

### Function `cpp::CMakeProject::configure`<a id="cpp::CMakeProject::configure"></a>

``` cpp
void configure();
```

Configures the cmake project.

Invokes "cmake \<source dir\>" command from the project binary dir. The signal CMakeProject::configureFinished(bool) is emitted when the configuration finishes.

-----

### Function `cpp::CMakeProject::buildTarget`<a id="cpp::CMakeProject::buildTarget"></a>

``` cpp
void buildTarget(const std::string& targetName);
```

Builds the given target.

Invokes "cmake --build \<binary dir\> --target \<target name\>" command.

The signal CMakeProject::stdoutLine() is emitted for every line sent to stdout from the cmake process. When the build finishes, CMakeProject::buildFinished() signal is emitted.

#### Parameter `cpp::CMakeProject::buildTarget::targetName`<a id="cpp::CMakeProject::buildTarget::targetName"></a>

``` cpp
const std::string& targetName
```

name of the target to build.

-----

### Function `cpp::CMakeProject::addTarget`<a id="cpp::CMakeProject::addTarget"></a>

``` cpp
CMakeTarget& addTarget(const std::string& name);
```

Adds a target to the set of project watched targets.

This function enables watching of target \<\\p name\>. The target properties are loaded from a `<\p name>.json` file in the root of the project binary dir (See CMakeTarget::Metadata::loadFromFile()). Passing the name of a target that does not belong to the project has undefined behavior (Loading of properties from JSON file may fail). After starting watch (See CMakeTarget::startWatch()) the target is rebuilt automaticallly whenever a source change occurs.

#### Parameter `cpp::CMakeProject::addTarget::name`<a id="cpp::CMakeProject::addTarget::name"></a>

``` cpp
const std::string& name
```

Name of the target to add to the project.

-----

### Function `cpp::CMakeProject::addTarget`<a id="cpp::CMakeProject::addTarget"></a>

``` cpp
CMakeTarget& addTarget(const CMakeTarget::Metadata& metadata);
```

Adds a target to the set of project watched targets.

This function enables watching of a target. Passing properties of a target that does not belong to the project has undefined behavior. After starting watch (See CMakeTarget::startWatch()) the target is rebuilt automaticallly whenever a source change occurs.

#### Parameter `cpp::CMakeProject::addTarget::metadata`<a id="cpp::CMakeProject::addTarget::metadata"></a>

``` cpp
const CMakeTarget::Metadata& metadata
```

Properties of the target to watch.

-----

### Function `cpp::CMakeProject::addSourceDirWatch`<a id="cpp::CMakeProject::addSourceDirWatch"></a>

``` cpp
efsw::WatchID addSourceDirWatch();
```

Registers a watch to a subtree of the source directory in the project internal filesystem watcher.

This function is invoked by CMakeTarget instances as part of their initialization to watch for target source changes.

-----

### Function `cpp::CMakeProject::addIncludeDirWatch`<a id="cpp::CMakeProject::addIncludeDirWatch"></a>

``` cpp
efsw::WatchID addIncludeDirWatch();
```

Registers a watch to a subtree of the source directory in the project internal filesystem watcher. The directory is an include directory.

This function is invoked by CMakeTarget instances as part of their initialization to watch for target include directories changes.

-----

### Function `cpp::CMakeProject::addBinaryDirWatch`<a id="cpp::CMakeProject::addBinaryDirWatch"></a>

``` cpp
efsw::WatchID addBinaryDirWatch();
```

Registers a watch to a subtree of the binary directory in the project internal filesystem watcher.

This function is invoked by CMakeTarget instances as part of their initialization to watch for changes in target build artifacts (Binaries, cache files, etc).

-----

### Function `cpp::CMakeProject::startWatch`<a id="cpp::CMakeProject::startWatch"></a>

``` cpp
void startWatch();
```

Starts target watching.

This function starts the internal filesystem watcher. Whenever a file event occurs, the internal filesystem watcher notifies registered targets, and them filter and react to events accordingly. Targets are directly-connected to the fs watcher (See SignalEmitter::connect()) so all target events (And their resulting actions) are processed and signaled from the watcher background thread.

-----

### Function `cpp::CMakeProject::configureFinished`<a id="cpp::CMakeProject::configureFinished"></a>

``` cpp
void configureFinished(bool successful);
```

Project configuration has finished.

#### Parameter `cpp::CMakeProject::configureFinished::successful`<a id="cpp::CMakeProject::configureFinished::successful"></a>

``` cpp
bool successful
```

True if the configuration was successful. False otherwise.

-----

### Function `cpp::CMakeProject::buildStarted`<a id="cpp::CMakeProject::buildStarted"></a>

``` cpp
void buildStarted(const std::string& target);
```

A target build started.

#### Parameter `cpp::CMakeProject::buildStarted::target`<a id="cpp::CMakeProject::buildStarted::target"></a>

``` cpp
const std::string& target
```

Name of the target being built.

-----

### Function `cpp::CMakeProject::buildFinished`<a id="cpp::CMakeProject::buildFinished"></a>

``` cpp
void buildFinished(const std::string& target, bool successful);
```

A target build has finished.

#### Parameter `cpp::CMakeProject::buildFinished::target`<a id="cpp::CMakeProject::buildFinished::target"></a>

``` cpp
const std::string& target
```

Name of the target built. \\param successful True if the build was successful. False otherwise.

-----

### Function `cpp::CMakeProject::stdoutLine`<a id="cpp::CMakeProject::stdoutLine"></a>

``` cpp
void stdoutLine(const std::string& targetName, const std::string& line);
```

stdout line feed from target build.

This signal is emitted for each line sent to stdout during a target build process.

#### Parameter `cpp::CMakeProject::stdoutLine::targetName`<a id="cpp::CMakeProject::stdoutLine::targetName"></a>

``` cpp
const std::string& targetName
```

Name of the target being build. \\param line Line from build process stdout.

-----

### Function `cpp::CMakeProject::fileSystemListener`<a id="cpp::CMakeProject::fileSystemListener"></a>

``` cpp
cpp::FileSystemListener& fileSystemListener();
```

Returns a reference to the internal filesystem listener.

This function is invoked by CMakeTargets to listen to changes on sources and include directories. Both the fs watcher and the listener are unique to the project.

-----

### Function `cpp::CMakeProject::sourceDir`<a id="cpp::CMakeProject::sourceDir"></a>

``` cpp
const std::string& sourceDir() const;
```

Returns the project source directory.

-----

### Function `cpp::CMakeProject::binaryDir`<a id="cpp::CMakeProject::binaryDir"></a>

``` cpp
const std::string& binaryDir() const;
```

Returns the project binary directory.

-----

-----
