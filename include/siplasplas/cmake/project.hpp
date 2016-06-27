#ifndef SIPLASPLAS_CMAKE_PROJECT_HPP
#define SIPLASPLAS_CMAKE_PROJECT_HPP

#include <string>
#include <siplasplas/signals/emitter.hpp>
#include <siplasplas/fswatch/fslistener.hpp>
#include "target.hpp"

namespace cpp
{

class CMakeProject : public SignalEmitter
{
public:
    CMakeProject(const std::string& sourceDir, const std::string& binaryDir);

    void configure();
    void buildTarget(const std::string& targetName);

    CMakeTarget& addTarget(const std::string& name);
    CMakeTarget& addTarget(const CMakeTarget::Metadata& metadata);
    efsw::WatchID addSourceDirWatch(const std::string& sourceDir);
    efsw::WatchID addIncludeDirWatch(const std::string& includeDir);
    efsw::WatchID addBinaryDirWatch(const std::string& binaryDir);
    void startWatch();

    void configureFinished(bool successful) {}
    void buildStarted(const std::string& target) {}
    void buildFinished(const std::string& target, bool successful) {}
    void stdoutLine(const std::string& targetName, const std::string& line) {}

    cpp::FileSystemListener& fileSystemListener();
    const std::string& sourceDir() const;
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
