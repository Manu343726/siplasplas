#include "project.hpp"
#include "logger.hpp"
#include <libexecstream/exec-stream.h>
#include <fmt/format.h>
#include <iostream>
#include <thread>

using namespace cpp;

CMakeProject::CMakeProject(const std::string& sourceDir, const std::string& binaryDir) :
    _sourceDir{sourceDir},
    _binaryDir{binaryDir}
{
    SignalEmitter::connect(_fileListener, &FileSystemListener::fileModified, *this, &CMakeProject::onFileChanged);
}

void CMakeProject::buildTarget(const std::string& targetName)
{
    exec_stream_t process;
    process.set_wait_timeout(exec_stream_t::s_all, 60*60*1000); // 1h timeout
    process.start("cmake", fmt::format("--build {0} --target {1}", _binaryDir, targetName));

    cpp::emit(*this).buildStarted(targetName);

    std::string line;
    while(std::getline(process.out(), line))
    {
        cpp::emit(*this).stdoutLine(line);
    }

    process.close();

    cpp::emit(*this).buildFinished(targetName, process.exit_code() == 0);
}

void CMakeProject::watchTarget(const std::string& name, const std::string& sourceDir, const std::string& includeDir)
{
    auto srcWatchId = _fileWatcher.addWatch(sourceDir, &_fileListener, false);
    _targetMap[srcWatchId] = name;

    cpp::cmake::log().info("Watching CMake target \"{}\" (src dir: {}, include dir: {}", name, sourceDir, includeDir);

    if(includeDir != "")
    {
        auto includeWatchId = _fileWatcher.addWatch(includeDir, &_fileListener, false);

        _targetMap[includeWatchId] = name;
    }
}

void CMakeProject::onFileChanged(efsw::WatchID watchId, const std::string& dir, const std::string& fileName)
{
    buildTarget(_targetMap[watchId]);
}

void CMakeProject::startWatch()
{
    _fileWatcher.watch();
}
