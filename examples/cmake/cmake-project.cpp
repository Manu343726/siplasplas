#include <siplasplas/cmake/project.hpp>

using namespace cpp;

int main()
{
    CMakeProject project(CMAKE_SOURCE_DIR, CMAKE_BINARY_DIR);

    project.buildTarget("examples-cmake-project");
}
