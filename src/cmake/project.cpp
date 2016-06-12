#include "project.hpp"
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
    exec_stream_t process{"cmake", fmt::format("cmake --build {0} --target {1}", _binaryDir, targetName)};

    std::string line;

    std::this_thread::sleep_for(std::chrono::seconds(10));

    while(std::getline(process.out(), line))
    {
        std::cout << line;
    }

    process.close();
    std::cout << "Exit code: " << process.exit_code() << std::endl;
}
