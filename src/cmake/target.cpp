#include "target.hpp"
#include "project.hpp"
#include "logger.hpp"

using namespace cpp;


CMakeTarget::CMakeTarget(CMakeProject& project, const std::string& name, const std::string& sourceDir) :
    CMakeTarget{
        project,
        CMakeTarget::Metadata{name, sourceDir, {},""}
    }
{}

CMakeTarget::CMakeTarget(CMakeProject& project, const CMakeTarget::Metadata& metadata) :
    _metadata{metadata},
    _project{project}
{
    for(const std::string& includeDir : _metadata.includeDirs)
    {
        _watchIds.push_back(
            _project.addIncludeDirWatch(includeDir)
        );
    }

    _watchIds.push_back(
        _project.addSourceDirWatch(_metadata.sourceDir)
    );

    SignalEmitter::connect(_project.fileSystemListener(), &FileSystemListener::fileModified, *this, &CMakeTarget::onFileChanged);
    SignalEmitter::connect(_project, &CMakeProject::buildStarted, *this, [this](const std::string& targetName)
    {
        if(targetName == name())
        {
            cpp::emit(*this).buildStarted();
        }
    });
    SignalEmitter::connect(_project, &CMakeProject::stdoutLine, *this, [this](const std::string& targetName, const std::string& line)
    {
        if(targetName == name())
        {
            cpp::emit(*this).stdoutLine(line);
        }
    });
    SignalEmitter::connect(_project, &CMakeProject::buildFinished, *this, [this](const std::string& targetName, bool successful)
    {
        if(targetName == name())
        {
            cpp::emit(*this).buildFinished(successful);
        }
    });
}

void CMakeTarget::build()
{
    _project.buildTarget(name());
}

const CMakeTarget::Metadata& CMakeTarget::metadata() const
{
    return _metadata;
}

const std::string& CMakeTarget::name() const
{
    return _metadata.name;
}

void CMakeTarget::onFileChanged(efsw::WatchID watchId, const std::string& dir, const std::string& fileName)
{
    if(std::find(_watchIds.begin(), _watchIds.end(), watchId) != _watchIds.end())
    {
        cpp::cmake::log().debug(
            "(cmake target '{}') file {}{} changed, ID {} found. Starting build...",
            name(), dir, fileName, watchId
        );
        build();
    }
}
