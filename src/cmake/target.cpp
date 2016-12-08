#include "target.hpp"
#include "project.hpp"
#include "logger.hpp"

#include <json.hpp>

using namespace cpp;

CMakeTarget::CMakeTarget(CMakeProject& project, const CMakeTarget::Metadata& metadata) :
    _metadata{metadata},
    _project{project},
    _building{false}
{
    _inputWatches.push_back(
        _project.addSourceDirWatch(_metadata.sourceDir)
    );

    for(const std::string& includeDir : _metadata.includeDirs)
    {
        _inputWatches.push_back(
            _project.addIncludeDirWatch(includeDir)
        );
    }

    _outputWatches.push_back(
        _project.addBinaryDirWatch(_metadata.binaryDir)
    );

    SignalEmitter::connect(_project.fileSystemListener(), &FileSystemListener::fileModified, *this, &CMakeTarget::onFileChanged);
    SignalEmitter::connect(_project, &CMakeProject::buildStarted, *this, [this](const std::string& targetName)
    {
        if(targetName == name())
        {
            _building = true;
            cpp::cmake::log().debug("(target {}) build started", targetName);
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
            _building = false;
            cpp::cmake::log().debug("(target {}) build finished (successful={})", targetName, successful);
            cpp::emit(*this).buildFinished(successful);
        }
    });

    // Remove errored watches (watchId < 0)
    filterErroredWatches(_inputWatches);
    filterErroredWatches(_outputWatches);
}

CMakeTarget::Metadata CMakeTarget::Metadata::loadFromFile(const CMakeProject& project, const std::string& file)
{
    const std::string path = project.binaryDir() + "/" + file + ".json";
    std::ifstream fileStream(path);

    if(fileStream)
    {
        auto json = nlohmann::json::parse(fileStream);
        CMakeTarget::Metadata metadata;

        metadata.name = json["NAME"];
        metadata.sourceDir = json["SOURCE_DIR"];
        metadata.binaryDir = json["BINARY_DIR"];

        for(const auto& elem : json["SOURCES"])
        {
            metadata.sources.push_back(elem);
        }
        for(const auto& elem : json["INCLUDE_DIRECTORIES"])
        {
            metadata.includeDirs.push_back(elem);
        }
        for(const auto& elem : json["INTERFACE_INCLUDE_DIRECTORIES"])
        {
            metadata.includeDirs.push_back(elem);
        }

        metadata.binary = json["LOCATION"];

        return metadata;
    }
    else
    {
        throw cpp::exception<std::runtime_error>("Cannot open target file {}", path);
    }
}

CMakeTarget CMakeTarget::loadFromFile(CMakeProject& project, const std::string& targetName)
{
    return {project, Metadata::loadFromFile(project, targetName)};
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
    if(std::find(_inputWatches.begin(), _inputWatches.end(), watchId) != _inputWatches.end())
    {
        cpp::cmake::log().debug(
            "(target {}) file {}{} changed, ID {} found. Starting build...",
            name(), dir, fileName, watchId
        );
        build();
    }
    else if(std::find(_outputWatches.begin(), _outputWatches.end(), watchId) != _outputWatches.end())
    {
        cpp::cmake::log().debug(
            "(target {}) File {}{} changed, ID {} found. Reload binary file",
            name(), dir, fileName, watchId
        );

        cpp::emit(*this).reloadBinary(dir + fileName);
    }
}

void CMakeTarget::filterErroredWatches(std::vector<efsw::WatchID>& watches)
{
    watches.erase(std::remove_if(watches.begin(), watches.end(), [](efsw::WatchID watchId)
    {
        return watchId < 0;
    }), watches.end());
}
