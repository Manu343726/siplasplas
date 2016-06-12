#ifndef SIPLASPLAS_CMAKE_PROJECT_HPP
#define SIPLASPLAS_CMAKE_PROJECT_HPP

#include <string>
#include <siplasplas/signals/emitter.hpp>
#include <siplasplas/fswatch/fslistener.hpp>

namespace cpp
{

class CMakeProject : public SignalEmitter
{
public:
    CMakeProject(const std::string& sourceDir, const std::string& binaryDir);

    void configure();
    void buildTarget(const std::string& targetName);

    void watchTarget(const std::string& name, const std::string& sourceDir, const std::string& includeDir = "");
    void startWatch();

    void configureFinished(bool successful) {}
    void buildStarted(const std::string& target) {}
    void buildFinished(const std::string& target, bool successful) {}
    void stdoutLine(const std::string& line) {}

private:
    std::string _sourceDir, _binaryDir;
    efsw::FileWatcher _fileWatcher;
    cpp::FileSystemListener _fileListener;
    std::unordered_map<efsw::WatchID, std::string> _targetMap;

    void onFileChanged(efsw::WatchID watchId, const std::string& dir, const std::string& fileName);
};

}

#include <reflection/include/siplasplas/cmake/project.hpp>

#endif // SIPLASPLAS_CMAKE_PROJECT_HPP
