---
layout: standardese-doc
---

# Header file `target.hpp`

``` cpp
#define SIPLASPLAS_CMAKE_TARGET_HPP 

#include <siplasplas/signals/emitter.hpp>

#include <siplasplas/fswatch/fslistener.hpp>

#include <memory>

#include <reflection/include/siplasplas/cmake/target.hpp>

namespace cpp
{
    class CMakeTarget;
}
```

## Class `cpp::CMakeTarget`<a id="cpp::CMakeTarget"></a>

``` cpp
class CMakeTarget
: public cpp::SignalEmitter
{
public:
    struct Metadata;
    
    CMakeTarget(CMakeProject& project, const Metadata& metadata);
    
    static CMakeTarget loadFromFile(CMakeProject& project, const std::string& file);
    
    void build();
    
    void buildStarted();
    
    void buildFinished(bool successful);
    
    void stdoutLine(const std::string& line);
    
    void reloadBinary(const std::string& binary);
    
    const Metadata& metadata() const;
    
    const std::string& name() const;
};
```

Represents a CMake target being watched by a project.

This class represents a CMake target that is registred in a CMakeProject for being watched and rebuilt automatically if any of its sources changes. This class has signals that notify target events (Build started, finished, etc) equivalent to those on CMakeProject but which are emitted for this target only (i.e. are not global to the project).

### Class `cpp::CMakeTarget::Metadata`<a id="cpp::CMakeTarget::Metadata"></a>

``` cpp
struct Metadata
{
    std::string name;
    
    std::string sourceDir;
    
    std::string binaryDir;
    
    std::vector<std::string> sources;
    
    std::vector<std::string> includeDirs;
    
    std::string binary;
    
    std::string binaryDebug;
    
    std::string binaryRelease;
    
    static Metadata loadFromFile(const CMakeProject& project, const std::string& targetName);
};
```

Represents the set of properties that define a target.

This structure is just an aggregation of the set of target properties CMakeTarget needs to work. Currently those are:

  - **name**: Name of the target.
  - **sourceDir**: Relative path to the target sources subtree.
  - **binaryDir**: Relative path to the target binary subtree.
  - **sources**: List of target sources.
  - **includeDirs**: List of target include directories.
  - **binary**: Absolute path to the target output binary.

#### Function `cpp::CMakeTarget::Metadata::loadFromFile`<a id="cpp::CMakeTarget::Metadata::loadFromFile"></a>

``` cpp
static Metadata loadFromFile(const CMakeProject& project, const std::string& targetName);
```

Loads target properties from a JSON file.

This function reads target properties from a JSON file named "\<\\p targetName\>.json" located at the root of the \\p project binary directory. The JSON file has a dictionary mapping from the name of the different target properties (NAME, SOURCES, INCLUDE\_DIRECTORIES, LOCATION) to their values. See [export\_target.cmake](https://github.com/Manu343726/siplasplas/blob/master/cmake/export_target.cmake).

Passing a target name that doesn't exist in the CMake project has undefined behavior.

@return An instance of CMakeTarget::Metadata with the read properties.

##### Parameter `cpp::CMakeTarget::Metadata::loadFromFile::project`<a id="cpp::CMakeTarget::Metadata::loadFromFile::project"></a>

``` cpp
const CMakeProject& project
```

Reference to the CMakeProject the target belongs to. \\param targetName Name of the target to load.

-----

-----

### Constructor `cpp::CMakeTarget::CMakeTarget`<a id="cpp::CMakeTarget::CMakeTarget"></a>

``` cpp
CMakeTarget(CMakeProject& project, const Metadata& metadata);
```

Creates a CMakeTarget.

During their initialization targets add the different source watches and connect for corresponding events in the project filesystem watcher. **The target is not registered in the project set**, so we suggest to never create isolated targets and call CMakeProject::addTarget() instead. (See CMakeTarget TODO).

#### Parameter `cpp::CMakeTarget::CMakeTarget::project`<a id="cpp::CMakeTarget::CMakeTarget::project"></a>

``` cpp
CMakeProject& project
```

Reference to the CMakeProject the target belongs to. \\param metadata Target properties.

-----

### Function `cpp::CMakeTarget::loadFromFile`<a id="cpp::CMakeTarget::loadFromFile"></a>

``` cpp
static CMakeTarget loadFromFile(CMakeProject& project, const std::string& file);
```

Loads a target from a file. See CMakeTarget::Metadata::loadFromFile().

#### Parameter `cpp::CMakeTarget::loadFromFile::project`<a id="cpp::CMakeTarget::loadFromFile::project"></a>

``` cpp
CMakeProject& project
```

Reference to the CMake project the target belongs to. \\param file JSON file containing target properties.

-----

### Function `cpp::CMakeTarget::build`<a id="cpp::CMakeTarget::build"></a>

``` cpp
void build();
```

Build the target.

This function invokes CMakeProject::build(\<target name\>). CMakeTarget::buildStarted() signal is emitted just after the build process starts. When the build process ends, CMakeTarget::buildFinished() is emitted with the result of the build.

-----

### Function `cpp::CMakeTarget::buildStarted`<a id="cpp::CMakeTarget::buildStarted"></a>

``` cpp
void buildStarted();
```

Target build has started.

-----

### Function `cpp::CMakeTarget::buildFinished`<a id="cpp::CMakeTarget::buildFinished"></a>

``` cpp
void buildFinished(bool successful);
```

Target build has finished.

#### Parameter `cpp::CMakeTarget::buildFinished::successful`<a id="cpp::CMakeTarget::buildFinished::successful"></a>

``` cpp
bool successful
```

True if the build was successful. False otherwise.

-----

### Function `cpp::CMakeTarget::stdoutLine`<a id="cpp::CMakeTarget::stdoutLine"></a>

``` cpp
void stdoutLine(const std::string& line);
```

stdout line feed from target build.

This signal is emitted for each line sent to stdout during a build process.

#### Parameter `cpp::CMakeTarget::stdoutLine::line`<a id="cpp::CMakeTarget::stdoutLine::line"></a>

``` cpp
const std::string& line
```

Line from build process stdout.

-----

### Function `cpp::CMakeTarget::reloadBinary`<a id="cpp::CMakeTarget::reloadBinary"></a>

``` cpp
void reloadBinary(const std::string& binary);
```

A binary artifact has changed and should be reloaded.

#### Parameter `cpp::CMakeTarget::reloadBinary::binary`<a id="cpp::CMakeTarget::reloadBinary::binary"></a>

``` cpp
const std::string& binary
```

Full path to the binary artifact.

-----

### Function `cpp::CMakeTarget::metadata`<a id="cpp::CMakeTarget::metadata"></a>

``` cpp
const Metadata& metadata() const;
```

Returns the target properties.

@return Const reference to the target properties.

-----

### Function `cpp::CMakeTarget::name`<a id="cpp::CMakeTarget::name"></a>

``` cpp
const std::string& name() const;
```

Returns the name of the target.

@return String containing the name of the target.

-----

-----
