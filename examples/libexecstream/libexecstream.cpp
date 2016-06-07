#include <libexecstream/exec-stream.h>
#include <iostream>
#include <algorithm>
#include <iterator>

int main()
{
    exec_stream_t cmake("cmake", "--version");
    std::string line;

    while(std::getline(cmake.out(), line))
    {
        std::cout << line << "\n";
    }
}
