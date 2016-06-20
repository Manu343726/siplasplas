#include <siplasplas/cmake/project.hpp>
#include <iostream>

using namespace cpp;

class CMakeProgress : public cpp::SignalEmitter
{
public:
    void onBuildStarted(const std::string& target)
    {
        std::cout << "Building " << target << "...\n";
    }

    void onBuildFinished(const std::string& target, bool successful)
    {
        if(successful)
        {
            std::cout << target << " successfully built.\n";
        }
        else
        {
            std::cout << target << " build failed.\n";
        }
    }

    void stdoutLineFromBuildJob(const std::string& target, const std::string& line)
    {
        std::cout << line << "\n";
    }
};

int main()
{
    const std::string& targetName = "pluginexample";
    CMakeProject project(CMAKE_SOURCE_DIR, CMAKE_BINARY_DIR);
    CMakeProgress cmakeProgress;

    auto& target = project.addTarget(targetName);

    SignalEmitter::connect_async(project, &CMakeProject::buildStarted, cmakeProgress, &CMakeProgress::onBuildStarted);
    SignalEmitter::connect_async(project, &CMakeProject::buildFinished, cmakeProgress, &CMakeProgress::onBuildFinished);
    SignalEmitter::connect_async(project, &CMakeProject::stdoutLine, cmakeProgress, &CMakeProgress::stdoutLineFromBuildJob);

    SignalEmitter::connect_async(target, &CMakeTarget::buildFinished, cmakeProgress, [&](bool successful)
    {
        std::cout << targetName << " build finished!!! (successful=" << std::boolalpha << successful << ")\n";
    });

    project.startWatch();

    while(true)
    {
        cmakeProgress.poll();
    }
}
