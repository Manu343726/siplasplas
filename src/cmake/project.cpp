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
{}

void CMakeProject::buildTarget(const std::string& targetName)
{
    exec_stream_t process;
    process.set_wait_timeout(exec_stream_t::s_all, 60*60*1000); // 1h timeout
    process.start("cmake", fmt::format("--build {0} --target {1}", _binaryDir, targetName));

    cpp::emit(*this).buildStarted(targetName);

    std::string line;
    while(std::getline(process.out(), line))
    {
        cpp::emit(*this).stdoutLine(targetName, line);
    }

    process.close();

    cpp::emit(*this).buildFinished(targetName, process.exit_code() == 0);
}

CMakeTarget& CMakeProject::addTarget(const std::string& targetName)
{
    return addTarget(CMakeTarget::Metadata::loadFromFile(*this, targetName));
}

CMakeTarget& CMakeProject::addTarget(const CMakeTarget::Metadata& metadata)
{
    auto it = std::find_if(_targets.begin(), _targets.end(), [&](const std::unique_ptr<CMakeTarget>& target)
    {
        return target->name() == metadata.name;
    });

    if(it == _targets.end())
    {
        cmake::log().debug("Adding target {} to project {}", metadata.name, _sourceDir);

        _targets.emplace_back( new CMakeTarget{*this, metadata});
        return *_targets.back();
    }
    else
    {
        throw cpp::exception<std::logic_error>(
            "A target named {} already exists in CMake project {}",
            metadata.name, _sourceDir
        );
    }
}

efsw::WatchID CMakeProject::addSourceDirWatch(const std::string& sourceDir)
{
    const auto watchId = _fileWatcher.addWatch(sourceDir, &_fileListener, true);

    if(watchId >= 0)
    {
        cmake::log().debug("Added source dir watch: {} (ID: {})", sourceDir, watchId);
    }

    return watchId;
}

efsw::WatchID CMakeProject::addIncludeDirWatch(const std::string& includeDir)
{
    const auto watchId = _fileWatcher.addWatch(includeDir, &_fileListener, true);

    if(watchId >= 0)
    {
        cmake::log().debug("Added include dir watch: {} (ID: {})", includeDir, watchId);
    }

    return watchId;
}

efsw::WatchID CMakeProject::addBinaryDirWatch(const std::string& binaryDir)
{
    const auto watchId = _fileWatcher.addWatch(binaryDir, &_fileListener, true);

    if(watchId >= 0)
    {
        cmake::log().debug("Added binary dir watch: {} (ID: {})", binaryDir, watchId);
    }

    return watchId;
}

cpp::FileSystemListener& CMakeProject::fileSystemListener()
{
    return _fileListener;
}

const std::string& CMakeProject::sourceDir() const
{
    return _sourceDir;
}

const std::string& CMakeProject::binaryDir() const
{
    return _binaryDir;
}

void CMakeProject::startWatch()
{
    _fileWatcher.watch();
}
