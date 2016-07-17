#ifndef SIPLASPLAS_CMAKE_PROJECT_HPP
#define SIPLASPLAS_CMAKE_PROJECT_HPP

#include <string>
#include <siplasplas/signals/emitter.hpp>
#include <siplasplas/fswatch/fslistener.hpp>
#include "target.hpp"

/**
 * \defgroup cmake
 * \brief A C++ API to configure and build CMake projects
 *
 * siplasplas-cmake provides a simple API to work with CMake projects.
 * **It's not a CMake instrospection API** but an API to configure and
 * build existing CMake projects with runtime C++ compilation in mind.
 *
 * The API has two different roles:
 *
 *  - **Building projects**: A CMakeProject takes a source directory
 *    and a binary directory to work with, giving methods to configure
 *    the project and build given targets. It works simply by issuing
 *    `cmake <source dir>` and `cmake --build <binary dir> --target <target>`
 *    commands.
 *
 *  - **Watching projects**: A CMake project can also keep track of given
 *    CMakeTarget instances to watch for target changes and rebuild them
 *    automatically.
 *
 * CMake targets can be described by a set of associated properties (Name,
 * source files, include directories, etc. See CMakeTarget::Metadata), or
 * by loading that properties directly from a JSON file (See CMakeTarget::Metadata::loadFromFile()).
 *
 * The API uses \refgroup signals to notify events related to the CMake projects,
 * such as a finished build, a build triggered by a source file change, etc.
 *
 * \example cmake/cmake-project.cpp
 *
 * This example shows how to set up a cmake project and one of its targets. Then
 * the target is watched for source changes and rebuild if needed.
 */

namespace cpp
{

/**
 * \ingroup cmake
 *
 *
 * \brief Configures and builds an existing CMake project.
 *
 * CMakeProject class handles an existing CMake project given
 * its source and build directories. The project can be configured and
 * built manually (See CMakeProject::configure() and CMakeProject::buildTarget() functions)
 * or targets can be registered to be watched and rebuilt on source changes.
 *
 * The class notifies project events through signals, such as when the configuration finished,
 * a target build started, etc.
 */
class CMakeProject : public SignalEmitter
{
public:

    /**
     * \brief Creates a cmake project given its source and build directories
     *
     * Source and build directories are inmutable and can only be set during
     * construction. Once the cmake project is initialized, target watching is
     * not launched by default and no targets are registered.
     *
     * \param sourceDir Path to the project source directory
     * \param binaryDir Path to the project binary directory
     */
    CMakeProject(const std::string& sourceDir, const std::string& binaryDir);

    /**
     * \brief Configures the cmake project.
     *
     * Invokes "cmake <source dir>" command from the project binary dir.
     * The signal CMakeProject::configureFinished(bool) is emitted when the configuration
     * finishes.
     */
    void configure();

    /**
     * \brief Builds the given target.
     *
     * Invokes "cmake --build <binary dir> --target <target name>" command.
     *
     * The signal CMakeProject::stdoutLine() is emitted for every line sent to stdout
     * from the cmake process. When the build finishes, CMakeProject::buildFinished() signal
     * is emitted.
     *
     * \param targetName name of the target to build.
     */
    void buildTarget(const std::string& targetName);

    /**
     * \brief Adds a target to the set of project watched targets.
     *
     * This function enables watching of target <\p name>. The target properties
     * are loaded from a `<\p name>.json` file in the root of the project binary dir
     * (See CMakeTarget::Metadata::loadFromFile()).
     * Passing the name of a target that does not belong to the project has undefined
     * behavior (Loading of properties from JSON file may fail).
     * After starting watch (See CMakeTarget::startWatch()) the target is rebuilt
     * automaticallly whenever a source change occurs.
     *
     * \param name Name of the target to add to the project.
     *
     * \return A CMakeTarget reference representing the target to watch.
     */
    CMakeTarget& addTarget(const std::string& name);

    /**
     * \brief Adds a target to the set of project watched targets.
     *
     * This function enables watching of a target.
     * Passing properties of a target that does not belong to the project
     * has undefined behavior.
     * After starting watch (See CMakeTarget::startWatch()) the target
     * is rebuilt automaticallly whenever a source change occurs.
     *
     * \param metadata Properties of the target to watch.
     *
     * \return A CMakeTarget reference representing the target to watch.
     */
    CMakeTarget& addTarget(const CMakeTarget::Metadata& metadata);

    /**
     * \brief Registers a watch to a subtree of the source directory in the
     * project internal filesystem watcher.
     *
     * This function is invoked by CMakeTarget instances as part of their
     * initialization to watch for target source changes.
     *
     * \param sourceDir Relative path to the subtree to watch (Relative to the
     * project source directory).
     *
     * \return a filesystem watch ID representing the watch (See fswatch).
     */
    efsw::WatchID addSourceDirWatch(const std::string& sourceDir);

    /**
     * \brief Registers a watch to a subtree of the source directory in the
     * project internal filesystem watcher. The directory is an include directory.
     *
     * This function is invoked by CMakeTarget instances as part of their
     * initialization to watch for target include directories changes.
     *
     * \param sourceDir Relative path to the subtree to watch (Relative to the
     * project source directory).
     *
     * \return a filesystem watch ID representing the watch (See fswatch).
     */
    efsw::WatchID addIncludeDirWatch(const std::string& includeDir);

    /**
     * \brief Registers a watch to a subtree of the binary directory in the
     * project internal filesystem watcher.
     *
     * This function is invoked by CMakeTarget instances as part of their
     * initialization to watch for changes in target build artifacts (Binaries,
     * cache files, etc).
     *
     * \param binaryDir Relative path to the subtree to watch (Relative to the
     * project binary directory).
     *
     * \return a filesystem watch ID representing the watch (See fswatch).
     */
    efsw::WatchID addBinaryDirWatch(const std::string& binaryDir);

    /**
     * \brief Starts target watching.
     *
     * This function starts the internal filesystem watcher.
     * Whenever a file event occurs, the internal filesystem watcher notifies
     * registered targets, and them filter and react to events accordingly.
     * Targets are directly-connected to the fs watcher (See SignalEmitter::connect()) so
     * all target events (And their resulting actions) are processed and signaled from the
     * watcher background thread.
     */
    void startWatch();

    /**
     * \name signals
     */
    ///@{

    /**
     * \brief Project configuration has finished.
     *
     * \param successful True if the configuration was successful. False otherwise.
     */
    void configureFinished(bool successful) {}

    /**
     * \brief A target build started.
     *
     * \param target Name of the target being built.
     */
    void buildStarted(const std::string& target) {}

    /**
     * \brief A target build has finished.
     *
     * \param target Name of the target built.
     * \param successful True if the build was successful. False otherwise.
     */
    void buildFinished(const std::string& target, bool successful) {}

    /**
     * \brief stdout line feed from target build.
     *
     * This signal is emitted for each line sent to stdout during a target
     * build process.
     *
     * \todo Rename to "buildProcessStdout() or something similar.
     *
     * \param targetName Name of the target being build.
     * \param line Line from build process stdout.
     */
    void stdoutLine(const std::string& targetName, const std::string& line) {}
    ///@} End group signals

    /**
     * \brief Returns a reference to the internal filesystem listener.
     *
     * This function is invoked by CMakeTargets to listen to changes on sources
     * and include directories. Both the fs watcher and the listener are unique to
     * the project.
     *
     * \return A reference to the internal FileSystemListener listening to project changes.
     */
    cpp::FileSystemListener& fileSystemListener();

    /**
     * \brief Returns the project source directory.
     *
     * \return Full path to the project source directory.
     */
    const std::string& sourceDir() const;

    /**
     * \brief Returns the project binary directory.
     *
     * \return Full path to the project binary directory.
     */
    const std::string& binaryDir() const;

private:
    std::string _sourceDir, _binaryDir;
    efsw::FileWatcher _fileWatcher;
    cpp::FileSystemListener _fileListener;
    std::vector<std::unique_ptr<CMakeTarget>> _targets;
};

}

#include <reflection/include/siplasplas/cmake/project.hpp>

#endif // SIPLASPLAS_CMAKE_PROJECT_HPP
