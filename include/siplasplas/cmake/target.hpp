#ifndef SIPLASPLAS_CMAKE_TARGET_HPP
#define SIPLASPLAS_CMAKE_TARGET_HPP

#include <siplasplas/signals/emitter.hpp>
#include <siplasplas/fswatch/fslistener.hpp>
#include <memory>

namespace cpp
{

class CMakeProject;

/**
 * \ingroup cmake
 *
 *
 * \brief Represents a CMake target being watched by a project.
 *
 * This class represents a CMake target that is registred in a CMakeProject
 * for being watched and rebuilt automatically if any of its sources changes.
 * This class has signals that notify target events (Build started, finished, etc)
 * equivalent to those on CMakeProject but which are emitted for this target
 * only (i.e. are not global to the project).
 *
 * \todo This class is highly coupled with the CMakeProject one. Maybe it
 * should be an implementation detail of CMakeTarget class instead, or at least
 * a member class. Creating targets isolated from projects has no sense (And is
 * actually error-prone since targets are not registred in projects during their
 * construction, registration is handled by CMakeProject::addTarget() functions).
 */
class CMakeTarget : public cpp::SignalEmitter
{
public:
    /**
     * \brief Represents the set of properties that define
     * a target.
     *
     * This structure is just an aggregation of the set of target
     * properties CMakeTarget needs to work. Currently those are:
     *
     * \todo Rename to "Properties".
     *
     *  - **name**: Name of the target.
     *  - **sourceDir**: Relative path to the target sources subtree.
     *  - **binaryDir**: Relative path to the target binary subtree.
     *  - **sources**: List of target sources.
     *  - **includeDirs**: List of target include directories.
     *  - **binary**: Absolute path to the target output binary.
     */
    struct Metadata
    {
        std::string name;
        std::string sourceDir;
        std::string binaryDir;
        std::vector<std::string> sources;
        std::vector<std::string> includeDirs;
        std::string binary;
        std::string binaryDebug;   /// \todo remove when merging fixing-windows-support
        std::string binaryRelease; /// \todo remove when merging fixing-windows-support

        /**
         * \brief Loads target properties from a JSON file.
         *
         * This function reads target properties from a JSON file named
         * "<\p targetName>.json" located at the root of the \p project binary
         * directory. The JSON file has a dictionary mapping from the name of
         * the different target properties (NAME, SOURCES, INCLUDE_DIRECTORIES,
         * LOCATION) to their values. See [export_target.cmake](https://github.com/Manu343726/siplasplas/blob/master/cmake/export_target.cmake).
         *
         * Passing a target name that doesn't exist in the CMake project has undefined
         * behavior.
         *
         * \todo Document `export_target.cmake` and the JSON file format.
         * \todo Throw an exception instead of UB?
         *
         * \param project Reference to the CMakeProject the target belongs to.
         * \param targetName Name of the target to load.
         *
         * @return An instance of CMakeTarget::Metadata with the read properties.
         */
        static Metadata loadFromFile(const CMakeProject& project, const std::string& targetName);
    };

    /**
     * \brief Creates a CMakeTarget.
     *
     * During their initialization targets add the different source
     * watches and connect for corresponding events in the project
     * filesystem watcher. **The target is not registered in the project set**,
     * so we suggest to never create isolated targets and call CMakeProject::addTarget()
     * instead. (See CMakeTarget TODO).
     *
     * \param project Reference to the CMakeProject the target belongs to.
     * \param metadata Target properties.
     */
    CMakeTarget(CMakeProject& project, const Metadata& metadata);


    /**
     * \brief Loads a target from a file. See CMakeTarget::Metadata::loadFromFile().
     *
     * \param project Reference to the CMake project the target belongs to.
     * \param file JSON file containing target properties.
     *
     * \return A CMakeTarget instance with the properties read from the file.
     */
    static CMakeTarget loadFromFile(CMakeProject& project, const std::string& file);

    /**
     * \brief Build the target.
     *
     * This function invokes CMakeProject::build(<target name>). CMakeTarget::buildStarted()
     * signal is emitted just after the build process starts. When the build process ends,
     * CMakeTarget::buildFinished() is emitted with the result of the build.
     */
    void build();

    /// \name signals
    ///@{

    /**
     * \brief Target build has started.
     */
    void buildStarted(){}

    /**
     * \brief Target build has finished.
     *
     * \param successful True if the build was successful. False otherwise.
     */
    void buildFinished(bool successful){}

    /**
     * \brief stdout line feed from target build.
     *
     * This signal is emitted for each line sent to stdout during a
     * build process.
     *
     * \todo Rename to "buildProcessStdout() or something similar.
     *
     * \param line Line from build process stdout.
     */
    void stdoutLine(const std::string& line){}

    /**
     * \brief A binary artifact has changed and should be reloaded.
     *
     * \param binary Full path to the binary artifact.
     */
    void reloadBinary(const std::string& binary){}
    ///@} End signals group

    /**
     * \brief Returns the target properties.
     *
     * \todo Rename to "properties".
     *
     * @return Const reference to the target properties.
     */
    const Metadata& metadata() const;

    /**
     * \brief Returns the name of the target.
     *
     * @return String containing the name of the target.
     */
    const std::string& name() const;

private:
    const Metadata _metadata;
    CMakeProject& _project;
    std::vector<efsw::WatchID> _inputWatches;
    std::vector<efsw::WatchID> _outputWatches;
    bool _building;

    void onFileChanged(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
    static void filterErroredWatches(std::vector<efsw::WatchID>& watches);
};

}

#include <reflection/include/siplasplas/cmake/target.hpp>

#endif // SIPLASPLAS_CMAKE_TARGET_HPP
