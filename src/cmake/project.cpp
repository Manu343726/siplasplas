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

CMakeTarget& CMakeProject::addTarget(const std::string& name, const std::string& sourceDir)
{
    return addTarget(CMakeTarget::Metadata{name, sourceDir, {}, ""});
}

CMakeTarget& CMakeProject::addTarget(const CMakeTarget::Metadata& metadata)
{
    auto it = std::find_if(_targets.begin(), _targets.end(), [&](const std::unique_ptr<CMakeTarget>& target)
    {
        return target->name() == metadata.name;
    });

    if(it == _targets.end())
    {
        cmake::log().debug("Adding target '{}' to CMake project '{}'", metadata.name, _sourceDir);

        _targets.emplace_back( new CMakeTarget{*this, metadata});
        return *_targets.back();
    }
    else
    {
        throw cpp::exception<std::logic_error>(
            "A target named '{}' already exists in CMake project '{}'",
            metadata.name, _sourceDir
        );
    }
}

efsw::WatchID CMakeProject::addSourceDirWatch(const std::string& sourceDir)
{
    const std::string path = fmt::format("{}/{}", _sourceDir, sourceDir);
    cmake::log().debug("Added source dir watch: {}", path);

    return _fileWatcher.addWatch(path, &_fileListener, true);
}

efsw::WatchID CMakeProject::addIncludeDirWatch(const std::string& includeDir)
{
    cmake::log().debug("Added include dir watch: {}", includeDir);

    return _fileWatcher.addWatch(includeDir, &_fileListener, true);
}

cpp::FileSystemListener& CMakeProject::fileSystemListener()
{
    return _fileListener;
}

void CMakeProject::startWatch()
{
    _fileWatcher.watch();
}
