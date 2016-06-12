#ifndef SIPLASPLAS_CMAKE_PROJECT_HPP
#define SIPLASPLAS_CMAKE_PROJECT_HPP

#include <string>

namespace cpp
{

class CMakeProject
{
public:
    CMakeProject(const std::string& sourceDir, const std::string& binaryDir);

    void configure();
    void buildTarget(const std::string& targetName);

private:
    std::string _sourceDir, _binaryDir;
};

}

#endif // SIPLASPLAS_CMAKE_PROJECT_HPP
