#ifndef SIPLASPLAS_CMAKE_TARGET_HPP
#define SIPLASPLAS_CMAKE_TARGET_HPP

#include <siplasplas/signals/emitter.hpp>
#include <siplasplas/fswatch/fslistener.hpp>
#include <memory>

namespace cpp
{

class CMakeProject;

class CMakeTarget : public cpp::SignalEmitter
{
public:
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

    CMakeTarget(CMakeProject& project, const Metadata& metadata);
    static CMakeTarget loadFromFile(CMakeProject& project, const std::string& file);

    void build();

    void buildStarted(){}
    void buildFinished(bool successful){}
    void stdoutLine(const std::string& line){}
    void reloadBinary(const std::string& binary){}

    const Metadata& metadata() const;
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
