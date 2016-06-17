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

    void stdoutLineFromBuildJob(const std::string& line)
    {
        std::cout << line << "\n";
    }
};

int main()
{
    CMakeProject project(CMAKE_SOURCE_DIR, CMAKE_BINARY_DIR);
    CMakeProgress cmakeProgress;

    project.watchTarget("run-examples-reflection", CMAKE_SOURCE_DIR "/examples/reflection");

    SignalEmitter::connect_async(project, &CMakeProject::buildStarted, cmakeProgress, &CMakeProgress::onBuildStarted);
    SignalEmitter::connect_async(project, &CMakeProject::buildFinished, cmakeProgress, &CMakeProgress::onBuildFinished);
    SignalEmitter::connect_async(project, &CMakeProject::stdoutLine, cmakeProgress, &CMakeProgress::stdoutLineFromBuildJob);

    project.startWatch();

    while(true)
    {
        cmakeProgress.poll();
    }
}
